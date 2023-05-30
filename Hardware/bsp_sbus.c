#include "string.h"
#include "stdlib.h"
#include "bsp_sbus.h"
#include "usart.h"
#include "main.h"
#include "stdio.h"
uint8_t   dbus_buf[DBUS_BUFLEN];
rc_info_t rc;


#define myabs(x) x>0?x:-x
/**
  * @brief      enable global uart it and do not use DMA transfer done it
  * @param[in]  huart: uart IRQHandler id
  * @param[in]  pData: receive buff
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef* huart, uint8_t* pData, uint32_t Size)
{
    uint32_t tmp1 = 0;

    tmp1 = huart->RxState;

    if (tmp1 == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0))
        {
            return HAL_ERROR;
        }

        huart->pRxBuffPtr = pData;
        huart->RxXferSize = Size;
        huart->ErrorCode  = HAL_UART_ERROR_NONE;

        /* Enable the DMA Stream */
        HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

        /*
         * Enable the DMA transfer for the receiver request by setting the DMAR bit
         * in the UART CR3 register
         */
        SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief      returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param[in]  dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
  *             to 7 to select the DMA Stream.
  * @retval     The number of remaining data units in the current DMAy Streamx transfer.
  */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(dma_stream->NDTR));
}

/**
  * @brief       handle received rc data
  * @param[out]  rc:   structure to save handled rc data
  * @param[in]   buff: the buff which saved raw rc data
  * @retval
  */
void rc_callback_handler(rc_info_t *rc, uint8_t *buff)
{
    int min=15;


#if DJI
    rc->ch1 = (buff[0] | buff[1] << 8) & 0x07FF;
    rc->ch1 -= 1024;
    rc->ch2 = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
    rc->ch2 -= 1024;
    rc->ch3 = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
    rc->ch3 -= 1024;
    rc->ch4 = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
    rc->ch4 -= 1024;

    rc->ch5 = ((buff[5] >> 4) & 0x000C) >> 2;
    rc->ch6 = (buff[5] >> 4) & 0x0003;

#else
  	rc->ch1 =((uint16_t)buff[1])|((uint16_t)((buff[2]&0x07)<<8));
  	rc->ch2 =((uint16_t)((buff[2]&0xf8)>>3))|(((uint16_t)(buff[3]&0x3f))<<5);
  	rc->ch3 =((uint16_t)((buff[3]&0xc0)>>6))|((((uint16_t)buff[4])<<2))|(((uint16_t)(buff[5]&0x01))<<10);
  	rc->ch4 =((uint16_t)((buff[5]&0xfe)>>1))|(((uint16_t)(buff[6]&0x0f))<<7);
  	rc->ch5=((uint16_t)((buff[6]&0xf0)>>4))|(((uint16_t)(buff[7]&0x7f))<<4);
  	rc->ch6=((uint16_t)((buff[7]&0x80)>>7))|(((uint16_t)buff[8])<<1)|(((uint16_t)(buff[9]&0x03))<<9);
  	rc->ch7=((uint16_t)((buff[9]&0xfc)>>2))|(((uint16_t)(buff[10]&0x1f))<<6);
  	rc->ch8=((uint16_t)((buff[10]&0xe0)>>5))|(((uint16_t)(buff[11]))<<3);
  	rc->ch9=((uint16_t)buff[12])|(((uint16_t)(buff[13]&0x07))<<8);
  	rc->ch10=((uint16_t)((buff[13]&0xf8)>>3))|(((uint16_t)(buff[14]&0x3f))<<5);
  	rc->ch11=((uint16_t)((buff[14]&0xc0)>>6))|(((uint16_t)buff[15])<<2)|(((uint16_t)(buff[16]&0x01))<<10);
  	rc->ch12=((uint16_t)((buff[16]&0xfe)>>1))|(((uint16_t)(buff[17]&0x0f))<<7);
  	rc->ch13=((uint16_t)((buff[17]&0xf0)>>4))|(((uint16_t)(buff[18]&0x7f))<<4);
  	rc->ch14=((uint16_t)((buff[18]&0x80)>>7))|(((uint16_t)buff[19])<<1)|(((uint16_t)(buff[20]&0x03))<<9);
  	rc->ch15=((uint16_t)((buff[20]&0xfc)>>2))|(((uint16_t)(buff[21]&0x1f))<<6);
		rc->ch16=((uint16_t)((buff[21]&0xe0)>>5))|(((uint16_t)buff[22])<<3);
	
		rc->ch1 -=1024;
		rc->ch2 -=1024;
		rc->ch3 -=1024;
		rc->ch4 -=1024;
		rc->ch13 -=1024;
		rc->ch14 -=1024;
		rc->ch15 -=1024;
		rc->ch16 -= 1024;
		
		sw_judge(rc);

#endif
    if(rc->ch1<min && rc->ch1>-min) rc->ch1=0;
    if(rc->ch2<min && rc->ch2>-min) rc->ch2=0;
    if(rc->ch3<min && rc->ch3>-min) rc->ch3=0;
    if(rc->ch4<min && rc->ch4>-min) rc->ch4=0;


}

/**
  * @brief      clear idle it flag after uart receive a frame data
  * @param[in]  huart: uart IRQHandler id
  * @retval
  */
static void uart_rx_idle_callback(UART_HandleTypeDef* huart)
{
    /* clear idle it flag avoid idle interrupt all the time */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* handle received data in idle interrupt */
    if (huart == &DBUS_HUART)
    {
        /* clear DMA transfer complete flag */
        __HAL_DMA_DISABLE(huart->hdmarx);

        /* handle dbus data dbus_buf from DMA */
        if ((DBUS_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == DBUS_BUFLEN)
        {
            rc_callback_handler(&rc, dbus_buf);
        }

        /* restart dma transmission */
        __HAL_DMA_SET_COUNTER(huart->hdmarx, DBUS_MAX_LEN);
        __HAL_DMA_ENABLE(huart->hdmarx);
    }
}


/**
  * @brief      callback this function when uart interrupt
  * @param[in]  huart: uart IRQHandler id
  * @retval
  */
void uart_receive_handler(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
            __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    {
        uart_rx_idle_callback(huart);
    }
}

/**
  * @brief   initialize dbus uart device
  * @param
  * @retval
  */
void dbus_uart_init(void)
{
    /* open uart idle it */
    __HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
    __HAL_UART_ENABLE_IT(&DBUS_HUART, UART_IT_IDLE);

    uart_receive_dma_no_it(&DBUS_HUART, dbus_buf, DBUS_MAX_LEN);
}

void sw_judge(rc_info_t *rc)
{
	if(rc->ch5 >1600) rc->ch5 = 1; else if(rc->ch5 <400) rc->ch5 = 3; else rc->ch5 = 2;
	if(rc->ch6 >1600) rc->ch6 = 1; else if(rc->ch6 <400) rc->ch6 = 3; else rc->ch6 = 2;
	if(rc->ch7 >1600) rc->ch7 = 1; else if(rc->ch7 <400) rc->ch7 = 3; else rc->ch7 = 2;
	if(rc->ch8 >1600) rc->ch8 = 1; else if(rc->ch8 <400) rc->ch8 = 3; else rc->ch8 = 2;
	if(rc->ch9 >1600) rc->ch9 = 1; else if(rc->ch9 <400) rc->ch9 = 3; else rc->ch9 = 2;
	if(rc->ch10 >1600) rc->ch10 = 1; else if(rc->ch10 <400) rc->ch10 = 3; else rc->ch10 = 2;
	if(rc->ch11 >1600) rc->ch11 = 1; else if(rc->ch11 <400) rc->ch11 = 3; else rc->ch11 = 2;
	if(rc->ch12 >1600) rc->ch12 = 1; else if(rc->ch12 <400) rc->ch12 = 3; else rc->ch12 = 2;
};


const rc_info_t *get_remote_control_point(void)
{
    return &rc;
}
