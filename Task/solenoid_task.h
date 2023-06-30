#ifndef __SOLENOID_TASK_H
#define __SOLENOID_TASK_H

#include "main.h"
#include "struct_typedef.h"

extern void solenoid_task(void const * argument);



#define SOL_3_Push_Ring_Pin GPIO_PIN_4//¼ÌµçÆ÷IN3
#define SOL_3_Push_Ring_GPIO_Port GPIOE

#define SOL_1_Fetch_Ring_Pin GPIO_PIN_0//¼ÌµçÆ÷IN2
#define SOL_1_Fetch_Ring_GPIO_Port GPIOF

#define SOL_2_Lifting_Ring_Pin GPIO_PIN_1//¼ÌµçÆ÷IN1
#define SOL_2_Lifting_Ring_GPIO_Port GPIOB

#define SOL_1_Shot_Pin GPIO_PIN_1//¼ÌµçÆ÷IN4
#define SOL_1_Shot_GPIO_Port GPIOB


#define SOL_Read_Push_Ring() HAL_GPIO_ReadPin(SOL_3_Push_Ring_GPIO_Port,SOL_3_Push_Ring_Pin)
#define SOL_Read_Fetch_Ring() HAL_GPIO_ReadPin(SOL_1_Fetch_Ring_GPIO_Port,SOL_1_Fetch_Ring_Pin)
#define SOL_Read_Lift_Ring() HAL_GPIO_ReadPin(SOL_2_Lifting_Ring_GPIO_Port,SOL_2_Lifting_Ring_Pin)
#define SOL_Read_Shot_Ring() HAL_GPIO_ReadPin(SOL_1_Shot_GPIO_Port,SOL_1_Shot_Pin)

#define SOL_ERR(x) printf("[Soleniod]: %s\r\n",x)

#define Limit_up() HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)
#define Limit_down() HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)

#define Fetch_Ring() do{\
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_SET)\
}while (0)


#define Lifting_Ring() do{\
	HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_SET)\
}while (0)

#define Push_Ring() do{\
	HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_SET)\
}while (0)

void Solenoid_End();
#endif