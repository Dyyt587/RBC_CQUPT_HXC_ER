/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "log.h"
#include "cmsis_os.h"
#include "stdbool.h"
#include "bldc_interface.h"
#include "bldc_interface_uart.h"
#include "kfifo.h"

Shell shell;
char shellBuffer[512];
char shelldata=0;
struct kfifo shell_rxfifo;
//static SemaphoreHandle_t shellMutex;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
	HAL_UART_Transmit(&huart6,(uint8_t*)data,len,0xffff);
	return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
	
	uint32_t cnt=kfifo_out(&shell_rxfifo,(uint8_t*)data,len);
	if(!cnt)osDelay(10);
	return cnt;
}
/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
	extern osMutexId shellMutexHandle;//由cubemx生成定义在freertos
	 // osMutexWait(shellMutexHandle,portMAX_DELAY);
   // xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
		extern osMutexId shellMutexHandle;//由cubemx生成定义在freertos
		//osMutexRelease(shellMutexHandle);
    //xSemaphoreGiveRecursive(shellMutex);
    return 0;
}



void uartLogWrite(char *buffer, short len);

Log uartLog = {
    .write = uartLogWrite,
    .active = true,
    .level = LOG_DEBUG
};
void uartLogWrite(char *buffer, short len)
{
    if (uartLog.shell)
    {
        shellWriteEndLine(uartLog.shell, buffer, len);
    }
}


/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
	//由cubemx生成故不再调用
//    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
		logRegister(&uartLog, &shell);
		static uint8_t rxbuffer[128]={0};
		kfifo_init(&shell_rxfifo,rxbuffer,sizeof(rxbuffer));
		HAL_UART_Receive_IT(&huart6,(uint8_t*)&shelldata,1);

    if (xTaskCreate(shellTask, "shell", 256, &shell, 15, NULL) != pdPASS)
    {
        logError("shell task creat failed");
    }
}


//CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);


