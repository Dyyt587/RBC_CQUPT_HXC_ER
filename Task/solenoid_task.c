#include "solenoid_task.h"
#include "cmsis_os.h"
#include "bsp_sbus.h"
#include "stdbool.h"
#include "stdio.h"
extern	rc_info_t rc;
unsigned char Ring_Flag=1;
unsigned char simple_rc_ctrl = 1;//遥控简单控制

//隐藏结构体
typedef enum{
	None=0U,
	Fetah_Will_Start,
	Lift_Will_Start,
	Push_Will_Start,
	Shot_Will_Start,
	
	Fetah_Runing_Start,
	Lift_Runing_Start,
	Push_Runing_Start,
	Shot_Runing_Start,
	
	Fetah_Started,
	Lift_Started,
	Push_Started,
	Shot_Started,
	
	
	Fetah_Will_End,
	Lift_Will_End,
	Push_Will_End,
	Shot_Will_End,
	
	Fetah_Runing_End,
	Lift_Runing_End,
	Push_Runing_End,
	Shot_Runing_End,
	
	Fetah_Ended,
	Lift_Ended,
	Push_Ended,
	Shot_Ended,
}Solenoid_State_e;

static Solenoid_State_e state;
void Solenoid_Delay(uint32_t time)
{
    //HAL_Delay(time);
    osDelay(time);
}


void Solenoid_Reset(void)
{
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
}
void Fetch_ring(uint8_t is_fetch)
{
	if(is_fetch)
		HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
}

void Lifting_ring(uint8_t is_lift)
{
	if(is_lift)
		HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
}

void Push_ring(uint8_t is_push)
{
	if(is_push)
		HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
}

void Solenoid_Init()
{
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
}

void Solenoid_Start()
{
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_SET);//取环装置下落
	Solenoid_Delay(2000); 
	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_SET);//取环装置张开
	Solenoid_Delay(2000); 
	
}
void Solenoid_End()
{

	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);//取环装置闭合
	Solenoid_Delay(2000); 
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);//取环装置回升
	Solenoid_Delay(2000); 
}
void test_io(void)
{
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
}
void solenoid_task(void const * argument)
{
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
	
	while (1) {
		osDelay(100);
		if (simple_rc_ctrl)
		{

			if (rc.ch5 == 1)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			else
			{
				//			HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
			}
			if (rc.ch6 == 1)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

				//			HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
				//			HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
				//			HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
			}
			else
			{
				//			HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
				//			HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
				//			HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
			}
			if (rc.ch8 == 1)
			{	

				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
			

				//			HAL_GPIO_WritePin(SOL_2_Lifting_Ring_GPIO_Port, SOL_2_Lifting_Ring_Pin, GPIO_PIN_RESET);
				//			HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
				//			HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
				
				//			HAL_GPIO_WritePin(SOL_3_Push_Ring_GPIO_Port, SOL_3_Push_Ring_Pin, GPIO_PIN_RESET);
				//			HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);
			}
			
			if(rc.ch10 ==1)
			{
					TIM8->CCR3 = 3600;
			}else
			{
								TIM8->CCR3 = 2000;
			}
			//		if(rc.ch11 == 1)
			//		{
			//			Solenoid_Start();
			//		}
			//		else if(rc.ch11 == 3)
			//		{
			//			Solenoid_End();
			//		}

		}else{

			
			//自动辅助控制
switch (state)
{

	case None:
		Solenoid_Reset();//相当于刚开机的初始状态
		break;
	case Fetah_Will_Start:

		state = Fetah_Runing_Start;
		Fetch_ring(1);
		break;
	case Lift_Will_Start:
		
		state = Lift_Runing_Start;
		break;
	case Push_Will_Start:
		
		state = Lift_Runing_Start;
		break;
	case Shot_Will_Start:
		
		state = Lift_Runing_Start;
		break;

	
//	case Fetah_Will_Start:
//		break;
//	case Fetah_Will_Start:
//		break;
//	case Fetah_Will_Start:
//		break;
//	case Fetah_Will_Start:
//		break;
//	case Fetah_Will_Start:
//		break;
//	case Fetah_Will_Start:
//		break;
//	case Fetah_Will_Start:
//		break;
	default:
		break;
}
		}
	
	}

	osDelay(10);
}



bool _IS_READY_FETCH(void)
{
	//仅需取环抓环推环电磁阀
	if((!SOL_Read_Fetch_Ring()) &&
		(!SOL_Read_Lift_Ring()) &&
		(!SOL_Read_Push_Ring()) //&&
		//(!SOL_Read_Shot_Ring())
	){
		return true;
	
	}else{
		//TODO: 将错误信息优化显示，比如添加输出到底是哪个电磁阀或条件导致的失败
		SOL_ERR("cant not fetch");
		return false;
	}
}
bool _IS_READY_LIFT(void)
{
	//仅需取环抓环推环电磁阀
	if((SOL_Read_Fetch_Ring()) &&
		(!SOL_Read_Lift_Ring()) &&
		(!SOL_Read_Push_Ring()) //&&
		//(!SOL_Read_Shot_Ring())
	){
		return true;
	
	}else{
		//TODO: 将错误信息优化显示，比如添加输出到底是哪个电磁阀或条件导致的失败
		SOL_ERR("cant not lift");
		return false;
	}
}
bool _IS_READY_PUSH(void)
{
	//仅需取环抓环推环电磁阀
	if((SOL_Read_Fetch_Ring()) &&
		(SOL_Read_Lift_Ring()) &&
		(!SOL_Read_Push_Ring()) &&
		(!SOL_Read_Shot_Ring())
	//TODO:
	//还要判断云台是否归位，还有舵机
	){
		return true;
	
	}else{
		//TODO: 将错误信息优化显示，比如添加输出到底是哪个电磁阀或条件导致的失败
		SOL_ERR("cant not push");
		return false;
	}
}
bool _IS_READY_SHOT(void)
{
	//仅需取环抓环推环电磁阀
	if(//(!SOL_Read_Fetch_Ring()) &&
		//(!SOL_Read_Lift_Ring()) &&
		//(!SOL_Read_Push_Ring()) //&&
		(!SOL_Read_Shot_Ring())
	//TODO:还要判断舵机是否归位
	){
		return true;
	
	}else{
		//TODO: 将错误信息优化显示，比如添加输出到底是哪个电磁阀或条件导致的失败
		SOL_ERR("cant not shot");
		return false;
	}
}
/////////////////////////////
//给外部的api函数
//统一格式，返回值不为0即当前不可执行该操作



//保证取环机构到位
uint8_t  Solenoid_Start_Fetch_ring(void)
{
	if(_IS_READY_FETCH())return 1;
	//通知线程工作
	state=Fetah_Will_Start;
}
//保证抓环机构到位
uint8_t  Solenoid_Start_Lift_ring(void)
{
	if(_IS_READY_LIFT())return 1;
	state=Lift_Will_Start;
}
//保证舵机云台和环到位
uint8_t  Solenoid_Start_Push_ring(void)
{
	if(_IS_READY_PUSH())return 1;
	state=Push_Will_Start;
}
//发射前会有一小段延时保证电机速度到位
uint8_t  Solenoid_Start_Shot_ring(void)
{
	if(_IS_READY_SHOT())return 1;
	state=Shot_Will_Start;

}

