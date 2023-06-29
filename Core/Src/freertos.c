/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId testTaskHandle;
osThreadId chassisTaskHandle;
osThreadId ledTaskHandle;
osThreadId solenoidTaskHandle;
osThreadId YuntaiTaskHandle;
osMutexId shellMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void test_task(void const * argument);
extern void chassis_task(void const * argument);
extern void led_task(void const * argument);
extern void solenoid_task(void const * argument);
void yuntaiTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* definition and creation of shellMutex */
  osMutexDef(shellMutex);
  shellMutexHandle = osMutexCreate(osMutex(shellMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of testTask */
  osThreadDef(testTask, test_task, osPriorityBelowNormal, 0, 128);
  testTaskHandle = osThreadCreate(osThread(testTask), NULL);

  /* definition and creation of chassisTask */
  osThreadDef(chassisTask, chassis_task, osPriorityNormal, 0, 128);
  chassisTaskHandle = osThreadCreate(osThread(chassisTask), NULL);

  /* definition and creation of ledTask */
  osThreadDef(ledTask, led_task, osPriorityIdle, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* definition and creation of solenoidTask */
  osThreadDef(solenoidTask, solenoid_task, osPriorityNormal, 0, 128);
  solenoidTaskHandle = osThreadCreate(osThread(solenoidTask), NULL);

  /* definition and creation of YuntaiTask */
  osThreadDef(YuntaiTask, yuntaiTask, osPriorityNormal, 0, 128);
  YuntaiTaskHandle = osThreadCreate(osThread(YuntaiTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_test_task */
/**
  * @brief  Function implementing the testTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_test_task */
__weak void test_task(void const * argument)
{
  /* USER CODE BEGIN test_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END test_task */
}

/* USER CODE BEGIN Header_yuntaiTask */
/**
* @brief Function implementing the YuntaiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_yuntaiTask */
__weak void yuntaiTask(void const * argument)
{
  /* USER CODE BEGIN yuntaiTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END yuntaiTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
