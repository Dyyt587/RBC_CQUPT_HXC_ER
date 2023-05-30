#include "bsp_as5048.h"
#include "spi.h"
#include "stdio.h"
#define AS5048_SPI       hspi4 /* for dji remote controler reciever */


#define __Read_NOP 0xc000
#define __Read_Clear_Error_Flag 0x4001
#define __Read_Angle     0xFFFF //¶ÁÈ¡½Ç¶ÈÃüÁî
////±àÂëÆ÷1
//#define __AS5048A1_CS_ENABLE()        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET)
//#define __AS5048A1_CS_DISABLE()       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)//Æ¬Ñ¡1ºÅSPIÊ¹¿É¶Á

//±àÂëÆ÷1
#define __AS5048A_CS_GPIO_Port_1			  GPIOI
#define __AS5048A_CS__Pin_1              GPIO_PIN_0
//±àÂëÆ÷2
#define __AS5048A_CS_GPIO_Port_2			  GPIOH
#define __AS5048A_CS__Pin_2              GPIO_PIN_12
//±àÂëÆ÷3
#define __AS5048A_CS_GPIO_Port_3			  GPIOH
#define __AS5048A_CS__Pin_3              GPIO_PIN_11
//±àÂëÆ÷4
#define __AS5048A_CS_GPIO_Port_4			  GPIOH
#define __AS5048A_CS__Pin_4              GPIO_PIN_10


//±àÂëÆ÷1
#define __AS5048A_CS_1_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_1, __AS5048A_CS__Pin_1, GPIO_PIN_RESET)
#define __AS5048A_CS_1_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_1, __AS5048A_CS__Pin_1, GPIO_PIN_SET)//Æ¬Ñ¡1ºÅSPIÊ¹¿É¶Á
//±àÂëÆ÷2
#define __AS5048A_CS_2_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_2, __AS5048A_CS__Pin_2, GPIO_PIN_RESET)
#define __AS5048A_CS_2_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_2, __AS5048A_CS__Pin_2, GPIO_PIN_SET)//Æ¬Ñ¡2ºÅSPIÊ¹¿É¶Á
//±àÂëÆ÷3
#define __AS5048A_CS_3_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_3, __AS5048A_CS__Pin_3, GPIO_PIN_RESET)
#define __AS5048A_CS_3_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_3, __AS5048A_CS__Pin_3, GPIO_PIN_SET)//Æ¬Ñ¡3ºÅSPIÊ¹¿É¶Á
//±àÂëÆ÷4
#define __AS5048A_CS_4_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_4, __AS5048A_CS__Pin_4, GPIO_PIN_RESET)
#define __AS5048A_CS_4_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_4, __AS5048A_CS__Pin_4, GPIO_PIN_SET)//Æ¬Ñ¡4ºÅSPIÊ¹¿É¶Á



//±àÂëÆ÷1
#define __AS5048A1_CS_ENABLE()        HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET)
#define __AS5048A1_CS_DISABLE()       HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET)//Æ¬Ñ¡1ºÅSPIÊ¹¿É¶Á
//±àÂëÆ÷2
#define __AS5048A2_CS_ENABLE()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_RESET)
#define __AS5048A2_CS_DISABLE()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET)//Æ¬Ñ¡1ºÅSPIÊ¹¿É¶Á
//±àÂëÆ÷3
#define __AS5048A3_CS_ENABLE()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET)
#define __AS5048A3_CS_DISABLE()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET)//Æ¬Ñ¡1ºÅSPIÊ¹¿É¶Á
//±àÂëÆ÷4
#define __AS5048A4_CS_ENABLE()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET)
#define __AS5048A4_CS_DISABLE()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET)//Æ¬Ñ¡1ºÅSPIÊ¹¿É¶Á




uint16_t command;
uint16_t angleValue;
uint16_t SPI_AS5048A_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A2_CS_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A2_CS_DISABLE();
    __AS5048A2_CS_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A2_CS_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}


float inline GET_AS5048A_ANGLE(uint16_t data)
{
    float angel;
    angel = (data*360.0)/16384;
    return angel;
}


uint16_t SPI_AS5048A_1_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_1_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_1_DISABLE();
    __AS5048A_CS_1_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_1_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

uint16_t SPI_AS5048A_2_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_2_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_2_DISABLE();
    __AS5048A_CS_2_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_2_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

uint16_t SPI_AS5048A_3_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_3_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_3_DISABLE();
    __AS5048A_CS_3_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_3_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

uint16_t SPI_AS5048A_4_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_4_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_4_DISABLE();
    __AS5048A_CS_4_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_4_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

void Test_AS5048(void)
{
    printf("1:%7.3f  2:%7.3f 3:%7.3f  4:%7.3f\r\n",
           GET_AS5048A_ANGLE(SPI_AS5048A_1_ReadData()),
           GET_AS5048A_ANGLE(SPI_AS5048A_2_ReadData()),
           GET_AS5048A_ANGLE(SPI_AS5048A_3_ReadData()),
           GET_AS5048A_ANGLE(SPI_AS5048A_4_ReadData())
          );
}
