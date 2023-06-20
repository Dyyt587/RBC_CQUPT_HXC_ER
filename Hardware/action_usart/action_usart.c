#include "action_usart.h"
#include "usart.h"
#include "stdarg.h"
#include "stdio.h"
////高效率发送Usart2
//unsigned char UartTxBuf_1[128];
void Usart2Printf(const char *format,...)
{
    unsigned char UartTxBuf[128];
    uint16_t len;
    va_list args;
    va_start(args,format);
    len = vsnprintf((char*)UartTxBuf,sizeof(UartTxBuf),(char*)format,args);
    va_end(args);
    HAL_UART_Transmit(&huart2, UartTxBuf, len,0xff);
}


/*
* 描述： HAL库实现多串口使用printf输出
* 参数： huart：对应的串口结构体
* 返回： 无*/
//void Uart_printf(UART_HandleTypeDef *huart,char *format, ...)
//{
//  unsigned char buf[512]; //定义临时数组，根据实际发送大小微调
// uint16_t len;
//  va_list args;
//  va_start(args, format);
//   len = vsnprintf((char *)buf, sizeof(buf), (char *)format+1, args);
//
//   va_end(args);
//  HAL_UART_Transmit(huart,(uint8_t *)buf,len,0xff);

//}