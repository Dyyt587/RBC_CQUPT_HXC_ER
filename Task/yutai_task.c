#include "led_task.h"
#include "cmsis_os.h"
#include "yuntai_task.h"
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
#include "test_task.h"


#define _SET_TRIPOD_HEAD_ICURRENT(xx) do{set_motor_A(&hcan2,0,xx,0,0);}while(0)
//#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1])*3.14f/18000.0f)
//#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1])/819200.0f)
#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1]))
#define GET_SPEED() motor_can2[1].speed_rpm
	
#define GET_SiGan_SPEED() motor_can2[2].speed_rpm
static unsigned int time;

float target=30;

float sigan_speed=0;
		
pid_t TripodHead_Position_pid;
pid_t TripodHead_Speed_pid;
		
pid_t SiGan_Speed_pid;
//电机1/19 导程4mm,控制单位mm
pid_t SiGan_Position_pid;
extern	rc_info_t rc;
	
	

void motors_init(void)
{

	vesc_init();
	vesc_set_point(2);//控制电机1和2
	PID_struct_init(&TripodHead_Position_pid,POSITION_PID,
					3000,1000,0.5f, 0.0f,0.4);
	
//	PID_struct_init(&SiGan_Speed_pid,POSITION_PID,
//					10000, 2000, 5.0f, 0.04f, 0.0f);
//	PID_struct_init(&SiGan_Position_pid,POSITION_PID,
//					10000, 2000, 5.0f, 0.04f, 0.0f);
//	
        PID_struct_init(&SiGan_Speed_pid,POSITION_PID, 10000, 2000,
                        4.5f, 0.08f, 1.5f);
        PID_struct_init(&SiGan_Position_pid,POSITION_PID, 10000, 2000,
                        100.0f, 0.00f, 10.0);
	
}
void pid_shoe(pid_t*pid)
{

	printf("p:%f,%f,%f",pid->get[NOW]
	,pid->set[NOW],pid->pos_out);
}
void motor_handler(const rc_info_t* _rc)
{

	target= (((float)_rc->ch15)*90.0f/673.f);
	float __target= pid_calc(&TripodHead_Position_pid, _GET_ANGLE_DEGREE(), target*819200/360.0f);//对读取到的角度进行偏移计算//在这里对地盘进行了读取并进行了pid计算，其中目标值为遥控器所给的输入//

    float sigantarget=0;//单位mm
	//sigan_speed= pid_calc(&SiGan_Position_pid, _GET_ANGLE_DEGREE(), sigantarget*8192/19/4);
	sigan_speed= pid_calc(&SiGan_Position_pid, _GET_ANGLE_DEGREE(), sigantarget*4/360);
	
	sigan_speed=rc.ch4*15.0;
	float __sigan_speed=pid_calc(&SiGan_Speed_pid,GET_SiGan_SPEED(),sigan_speed);//
	set_motor_A(&hcan2,0,__target,__sigan_speed,0);//
	
} 

void steer_and_5065motor_handler(const rc_info_t* _rc)
{
       if(_rc->ch12==1)//通道12为发射机构加速和发射
		{
			float data = ((float)rc.ch16+672)*50; //

			vesc_set_rmp(data);	
			//vesc_set_rmp(2000);	

		}
		else
		{
			TIM8 ->CCR3 = 2000;//5%
			//TIM8 ->CCR4 = 2000;
//			vesc_set_rmp(0);	
		}
		if(_rc->ch10 == 3)//初始化通道10处于1，舵机的PWM为1000
		{
			TIM4->CCR4 = 1000;
		}
		else if(_rc->ch10 == 1)
		{
			TIM4->CCR4 = 2140;//将舵机提上来，以便接环
		}

}
void yuntaiTask(void const * argument)
{
	const rc_info_t* _rc = get_remote_control_point();
	motors_init();
	for(;;)
	{
	  	steer_and_5065motor_handler(_rc);
		motor_handler(_rc);
		osDelay(5);
	}
//	const rc_info_t* _rc = get_remote_control_point();
//	vesc_init();
//	vesc_set_point(2);//控制电机1和2
//	
//	PID_struct_init(&TripodHead_Position_pid,POSITION_PID,3000,1000,0.5f, 0.0f,0.4);
//	
//	PID_struct_init(&SiGan_Speed_pid,POSITION_PID, 10000, 2000,
//                        5.0f, 0.04f, 0.0f);
//	int time_count = 0;
//	Init_test_AS5048();
//	while(1)
//	{
//		extern int a;
//		Test_AS5048();
//		if(rc.ch12==1)//通道12为发射机构加速和发射
//		{
//			float data = ((float)rc.ch16+672)*50;
//			vesc_set_rmp(data);	
//		}
//		else
//		{
//			TIM8 ->CCR3 = 2000;//5%
//			TIM8 ->CCR4 = 2000;
//			time_count =0;
//	
//		}
//		if(rc.ch10 == 3)//初始化通道10处于1，舵机的PWM为1000
//		{
//			TIM4->CCR4 = 1000;
//		}
//		else if(rc.ch10 == 1)
//		{
//			TIM4->CCR4 = 2140;//将舵机提上来，以便接环
//		}


//		target= (((float)_rc->ch15)*90.0f/673.f);

//		float __target= pid_calc(&TripodHead_Position_pid, _GET_ANGLE_DEGREE(), target*819200/360.0f);//对读取到的角度进行偏移计算//在这里对地盘进行了读取并进行了pid计算，其中目标值为遥控器所给的输入//
//		
//		sigan_speed=rc.ch4*15.0;
//		float __sigan_speed=pid_calc(&SiGan_Speed_pid,GET_SiGan_SPEED(),sigan_speed);//
//		
//		set_motor_A(&hcan2,0,__target,__sigan_speed,0);//

//		osDelay(5);

//	}
}



