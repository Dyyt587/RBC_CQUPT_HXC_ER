/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_can.h"
#include "bsp_sbus.h"
#include "stdio.h"
#include "vofa_lower.h"
#include "flashdb.h"
#include "log.h"
#include "commend.h"
#include <string.h>

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};

static struct fdb_default_kv_node default_kv_table[] = {
        {"username", "armink", 0}, /* string KV */
        {"password", "123456", 0}, /* string KV */
        {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
        {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
//重写printf
int fputc(int ch,FILE *f)
{
    uint8_t temp[1]= {ch};
    HAL_UART_Transmit(&huart6,temp,1,2);
		return 0;
}


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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define FDB_LOG_TAG "[sample][kvdb][basic]"

void kvdb_basic_sample(fdb_kvdb_t kvdb)
{
    struct fdb_blob blob;
    int boot_count = 0;

    FDB_INFO("==================== kvdb_basic_sample ====================\n");

    { /* GET the KV value */
        /* get the "boot_count" KV value */
        fdb_kv_get_blob(kvdb, "boot_count", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'boot_count' value is %d\n", boot_count);
        } else {
            FDB_INFO("get the 'boot_count' failed\n");
        }
    }

    { /* CHANGE the KV value */
        /* increase the boot count */
        boot_count ++;
        /* change the "boot_count" KV's value */
        fdb_kv_set_blob(kvdb, "boot_count", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        FDB_INFO("set the 'boot_count' value to %d\n", boot_count);
    }

    FDB_INFO("===========================================================\n");
}

#define FDB_LOG_TAG "[sample][kvdb][string]"

void kvdb_type_string_sample(fdb_kvdb_t kvdb)
{
    FDB_INFO("==================== kvdb_type_string_sample ====================\n");

    { /* CREATE new Key-Value */
        char temp_data[10] = "36C";

        /* It will create new KV node when "temp" KV not in database. */
        fdb_kv_set(kvdb, "temp", temp_data);
        FDB_INFO("create the 'temp' string KV, value is: %s\n", temp_data);
    }

    { /* GET the KV value */
        char *return_value, temp_data[10] = { 0 };

        /* Get the "temp" KV value.
         * NOTE: The return value saved in fdb_kv_get's buffer. Please copy away as soon as possible.
         */
        return_value = fdb_kv_get(kvdb, "temp");
        /* the return value is NULL when get the value failed */
        if (return_value != NULL) {
            strncpy(temp_data, return_value, sizeof(temp_data));
            FDB_INFO("get the 'temp' value is: %s\n", temp_data);
        }
    }

    { /* CHANGE the KV value */
        char temp_data[10] = "38C";

        /* change the "temp" KV's value to "38.1" */
        fdb_kv_set(kvdb, "temp", temp_data);
        FDB_INFO("set 'temp' value to %s\n", temp_data);
    }

    { /* DELETE the KV by name */
        fdb_kv_del(kvdb, "temp");
        FDB_INFO("delete the 'temp' finish\n");
    }

    FDB_INFO("===========================================================\n");
}


void kvdb_type_blob_sample(fdb_kvdb_t kvdb)
{
    struct fdb_blob blob;

    FDB_INFO("==================== kvdb_type_blob_sample ====================\n");

    { /* CREATE new Key-Value */
        int temp_data = 36;

        /* It will create new KV node when "temp" KV not in database.
         * fdb_blob_make: It's a blob make function, and it will return the blob when make finish.
         */
        fdb_kv_set_blob(kvdb, "temp", fdb_blob_make(&blob, &temp_data, sizeof(temp_data)));
        FDB_INFO("create the 'temp' blob KV, value is: %d\n", temp_data);
    }

    { /* GET the KV value */
        int temp_data = 0;

        /* get the "temp" KV value */
        fdb_kv_get_blob(kvdb, "temp", fdb_blob_make(&blob, &temp_data, sizeof(temp_data)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'temp' value is: %d\n", temp_data);
        }
    }

    { /* CHANGE the KV value */
        int temp_data = 38;

        /* change the "temp" KV's value to 38 */
        fdb_kv_set_blob(kvdb, "temp", fdb_blob_make(&blob, &temp_data, sizeof(temp_data)));
        FDB_INFO("set 'temp' value to %d\n", temp_data);
    }

    { /* DELETE the KV by name */
        fdb_kv_del(kvdb, "temp");
        FDB_INFO("delete the 'temp' finish\n");
    }

    FDB_INFO("===========================================================\n");
}


static struct fdb_kvdb kvdb = { 0 };

static void lock(fdb_db_t db)
{
    //__disable_irq();
}

static void unlock(fdb_db_t db)
{
    //__enable_irq();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SPI4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_UART7_Init();
  MX_UART8_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  dbus_uart_init();
  TIM8 ->CCR3 = 0;
  TIM8 ->CCR4 = 0;
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
  TIM4->CCR4 = 800;
	extern void userShellInit(void);//由于该rtos不支持宏自动初始化,故手动初始化
  userShellInit();
  my_can_filter_init_recv_all(&hcan1);
  can_filter_recv_special(&hcan2);
  var_init();
  
fdb_err_t result;


        struct fdb_default_kv default_kv;

        default_kv.kvs = default_kv_table;
        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
        /* set the lock and unlock function if you want */
        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);
        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */



        /* run basic KV samples */
      //  kvdb_basic_sample(&kvdb);
        /* run string KV samples */
      //  kvdb_type_string_sample(&kvdb);
        /* run blob KV samples */
       // kvdb_type_blob_sample(&kvdb);


  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
        //result = fdb_kvdb_init(&kvdb, "env", "fdb_kvdb1", &default_kv, NULL);

		
		   /* run basic KV samples */
       //kvdb_basic_sample(&kvdb);
        /* run string KV samples */
       //kvdb_type_string_sample(&kvdb);
        /* run blob KV samples */
       //kvdb_type_blob_sample(&kvdb);
  while (1)
  {
    /* USER CODE END WHILE */
			printf("flashdb ");

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
