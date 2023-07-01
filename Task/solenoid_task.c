#include "solenoid_task.h"
#include "cmsis_os.h"
#include "bsp_sbus.h"
#include "stdbool.h"
#include "stdio.h"
extern	rc_info_t rc;
unsigned char Ring_Flag=1;
unsigned char simple_rc_ctrl = 1;//ң�ؼ򵥿���
extern uint8_t shot_flag;

//���ؽṹ��
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
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_SET);//ȡ��װ������
	Solenoid_Delay(2000); 
	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_SET);//ȡ��װ���ſ�
	Solenoid_Delay(2000); 
	
}
void Solenoid_End()
{

	HAL_GPIO_WritePin(SOL_1_Shot_GPIO_Port, SOL_1_Shot_Pin, GPIO_PIN_RESET);//ȡ��װ�ñպ�
	Solenoid_Delay(2000); 
	HAL_GPIO_WritePin(SOL_1_Fetch_Ring_GPIO_Port, SOL_1_Fetch_Ring_Pin, GPIO_PIN_RESET);//ȡ��װ�û���
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
				state = 0;
	while (1) {
		osDelay(100);
		if(rc.ch11 == 1)
		{
			simple_rc_ctrl=1;
		}else
		{
			simple_rc_ctrl=0;			
		}
		if (simple_rc_ctrl)
		{

			if (rc.ch5 == 1)//�价
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);//liang
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
			}
			
			if (rc.ch6 == 1) //���� ch4
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
			}
			
			if (rc.ch8 == 1) //�ƻ�
			{	
				TIM8->CCR3 = 3600;
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
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
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);	
		//	printf();
			//�Զ���������
switch (state)
{

	case 0:
//		if(Limit_up() == 1)
//		{
//			//����˿�˲��������ƶ�
//			
//			state = 1;
//		}else
//		{
//			state = 1;
//		//	state = 0;
//		}
		state = 1;
		break;
	case 1:
		shot_flag = 1;
	  while(shot_flag)
		{
				TIM8->CCR3 = 3600; //̧����
			//�ƻ�
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);	

		}
		shot_flag = 0;
//		Fetch_ring(1);
		state = 2;
		break;
	case 2:
		shot_flag = 1;
		TIM8->CCR3 = 2000; //���	 
	  osDelay(500);
	  while(shot_flag)
		{
			//she��
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);	

		}
		shot_flag = 0;		
		state = 3;
//		state = Lift_Runing_Start;
		break;
	case 3:
		shot_flag = 1;
	  while(shot_flag)
		{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);	
		}
		shot_flag=0;
		state = 0;
		break;
//	case Shot_Will_Start:
//		
//		state = Lift_Runing_Start;
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
	//����ȡ��ץ���ƻ���ŷ�
	if((!SOL_Read_Fetch_Ring()) &&
		(!SOL_Read_Lift_Ring()) &&
		(!SOL_Read_Push_Ring()) //&&
		//(!SOL_Read_Shot_Ring())
	){
		return true;
	
	}else{
		//TODO: ��������Ϣ�Ż���ʾ�������������������ĸ���ŷ����������µ�ʧ��
		SOL_ERR("cant not fetch");
		return false;
	}
}
bool _IS_READY_LIFT(void)
{
	//����ȡ��ץ���ƻ���ŷ�
	if((SOL_Read_Fetch_Ring()) &&
		(!SOL_Read_Lift_Ring()) &&
		(!SOL_Read_Push_Ring()) //&&
		//(!SOL_Read_Shot_Ring())
	){
		return true;
	
	}else{
		//TODO: ��������Ϣ�Ż���ʾ�������������������ĸ���ŷ����������µ�ʧ��
		SOL_ERR("cant not lift");
		return false;
	}
}
bool _IS_READY_PUSH(void)
{
	//����ȡ��ץ���ƻ���ŷ�
	if((SOL_Read_Fetch_Ring()) &&
		(SOL_Read_Lift_Ring()) &&
		(!SOL_Read_Push_Ring()) &&
		(!SOL_Read_Shot_Ring())
	//TODO:
	//��Ҫ�ж���̨�Ƿ��λ�����ж��
	){
		return true;
	
	}else{
		//TODO: ��������Ϣ�Ż���ʾ�������������������ĸ���ŷ����������µ�ʧ��
		SOL_ERR("cant not push");
		return false;
	}
}
bool _IS_READY_SHOT(void)
{
	//����ȡ��ץ���ƻ���ŷ�
	if(//(!SOL_Read_Fetch_Ring()) &&
		//(!SOL_Read_Lift_Ring()) &&
		//(!SOL_Read_Push_Ring()) //&&
		(!SOL_Read_Shot_Ring())
	//TODO:��Ҫ�ж϶���Ƿ��λ
	){
		return true;
	
	}else{
		//TODO: ��������Ϣ�Ż���ʾ�������������������ĸ���ŷ����������µ�ʧ��
		SOL_ERR("cant not shot");
		return false;
	}
}
/////////////////////////////
//���ⲿ��api����
//ͳһ��ʽ������ֵ��Ϊ0����ǰ����ִ�иò���



//��֤ȡ��������λ
uint8_t  Solenoid_Start_Fetch_ring(void)
{
	if(_IS_READY_FETCH())return 1;
	//֪ͨ�̹߳���
	state=Fetah_Will_Start;
}
//��֤ץ��������λ
uint8_t  Solenoid_Start_Lift_ring(void)
{
	if(_IS_READY_LIFT())return 1;
	state=Lift_Will_Start;
}
//��֤�����̨�ͻ���λ
uint8_t  Solenoid_Start_Push_ring(void)
{
	if(_IS_READY_PUSH())return 1;
	state=Push_Will_Start;
}
//����ǰ����һС����ʱ��֤����ٶȵ�λ
uint8_t  Solenoid_Start_Shot_ring(void)
{
	if(_IS_READY_SHOT())return 1;
	state=Shot_Will_Start;

}

