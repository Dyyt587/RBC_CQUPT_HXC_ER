#include "yuntai.h"
#include "led_task.h"
#include "cmsis_os.h"
#include "test_task.h"
#include "bsp_as5048.h"
#include "vofa_lower.h"
#include "bldc_interface.h"
#include "bsp_sbus.h"
#include "bsp_vesc.h"
#include "can.h"
#include "bsp_can.h"
#include "stdio.h"
#include "pid.h"
#include "vofa_lower.h"
#include "shell.h"
#include "commend.h"
#include <stdarg.h>
#include <stdlib.h>
#include "log.h"

#define _SET_TRIPOD_HEAD_ICURRENT(xx) do{set_motor_A(&hcan2,0,xx,0,0);}while(0)
//#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1])*3.14f/18000.0f)
//#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1])/819200.0f)
#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1]))
#define GET_SPEED() motor_can2[1].speed_rpm
	
#define GET_SiGan_SPEED() motor_can2[2].speed_rpm
pid_t TripodHead_Position_pid;
pid_t TripodHead_Speed_pid;


pid_t SiGan_Speed_pid;

void yuntai_task(void const * argument)
{
	const rc_info_t* _rc = get_remote_control_point();
	vesc_init();
	vesc_set_point(2);//���Ƶ��1��2

	PID_struct_init(&TripodHead_Position_pid,POSITION_PID,3000,1000,0.5f, 0.0f,0.4);
	
	PID_struct_init(&SiGan_Speed_pid,POSITION_PID, 10000, 2000,
                        5.0f, 0.04f, 0.0f);
	int time_count = 0;

	while(1)
	{
		float target=30;

		target= (((float)_rc->ch15)*90.0f/673.f);
		float __target= pid_calc(&TripodHead_Position_pid, _GET_ANGLE_DEGREE(), target*819200/360.0f);//�Զ�ȡ���ĽǶȽ���ƫ�Ƽ���//������Ե��̽����˶�ȡ��������pid���㣬����Ŀ��ֵΪң��������������//
		float sigan_speed=_rc->ch4*15.0;
		float __sigan_speed=pid_calc(&SiGan_Speed_pid,GET_SiGan_SPEED(),sigan_speed);//
		set_motor_A(&hcan2,0,__target,__sigan_speed,0);//

		
		if(_rc->ch12==1)//ͨ��12Ϊ����������ٺͷ���
		{
			float data = ((float)_rc->ch16+672)*50;//���

			vesc_set_rmp(data);	
			//vesc_set_rmp(2000);	
		}else{
			TIM8 ->CCR3 = 2000;//5%
			TIM8 ->CCR4 = 2000;
			time_count =0;
//			vesc_set_rmp(0);	
		}
		if(_rc->ch10 == 3)//��ʼ��ͨ��10����1�������PWMΪ1000
		{
			TIM4->CCR4 = 1000;
		}
		else if(_rc->ch10 == 1)
		{
			TIM4->CCR4 = 2140;//��������������Ա�ӻ�
		}
		osDelay(5);
	}


}
	