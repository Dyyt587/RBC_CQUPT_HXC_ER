
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
		#define _SET_TRIPOD_HEAD_ICURRENT(xx) do{set_motor_A(&hcan2,0,xx,0,0);}while(0)
		//#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1])*3.14f/18000.0f)
		//#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1])/819200.0f)
		#define _GET_ANGLE_DEGREE() ((float)get_total_angle(&motor_can2[1]))
#define GET_SPEED() motor_can2[1].speed_rpm
static unsigned int time;
float __point[3];//测试使用，present，out，target
float target=30;

pid_t TripodHead_Position_pid;
pid_t TripodHead_Speed_pid;
extern	rc_info_t rc;
void test_task(void const * argument)
{
//	var_init();
	const rc_info_t* _rc = get_remote_control_point();
	vesc_init();
	vesc_set_point(2);//控制电机1和2
//	PID_struct_init(&TripodHead_Speed_pid,POSITION_PID,3000,1000,50.0f, 0.001f,0.0f);
	PID_struct_init(&TripodHead_Position_pid,POSITION_PID,3000,1000,0.5f, 0.0f,0.4);
	int time_count = 0;

	while(1)
	{
//printf("fdata \r\n");

//		Test_AS5048();
//vofa_send_protocol("a",&data,1);
//		target= ((float)rc->ch15)*90.0f/673.f;
//		printf("target=%f\r\n",target);
//		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_14);
//		osDelay(100);
//		time++;
//		//bldc_interface_get_values();
//		if(time%2==0){	
		//vesc_set_rmp(5000);
		if(rc.ch12==1)//通道12为发射机构加速和发射
		{
			float data = ((float)rc.ch16+672)*40;
			printf("fdata %f\r\n",data);
			//在2000-4000之间
//			float data = ((float)rc.ch16+500)/1000*2000+2000;
//			if(data>4000)data=4000;
//			if(data<2000)data=2000;
//			TIM8 ->CCR3 = data;
//			TIM8 ->CCR4 = data;
			vesc_set_rmp(data);	
			//vesc_set_rmp(2000);	
//			time_count++;
//			if(time_count == 100)
//			{
//				time_count =0;
//							//发射
//			}
		}
		else
		{
			TIM8 ->CCR3 = 2000;//5%
			TIM8 ->CCR4 = 2000;
			time_count =0;
//			vesc_set_rmp(0);	
		}
		if(rc.ch10 == 3)//初始化通道10处于1，舵机的PWM为1000
		{
			TIM4->CCR4 = 1000;
		}else if(rc.ch10 == 1)
		{
			TIM4->CCR4 = 2140;//将舵机提上来，以便接环
		}
//		}
//		float present=_GET_ANGLE_DEGREE();
		target= (((float)_rc->ch15)*90.0f/673.f);
//		printf("tar=%f\r\n",target);
		float __target= pid_calc(&TripodHead_Position_pid, _GET_ANGLE_DEGREE(), target*819200/360.0f);
//		float out = pid_calc(&TripodHead_Speed_pid, GET_SPEED(), __target);
//		//printf("out = %f",out);
			_SET_TRIPOD_HEAD_ICURRENT(__target);

		osDelay(5);

	}
}

void setkp( float data)
{
	TripodHead_Position_pid.p=data;
	printf("p= %f\r\n",TripodHead_Position_pid.p);
}
SHELL_EXPORT_CMD_AGENCY(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
kp, setkp, test,SHELL_PARAM_FLOAT(p1));

void setki( float data)
{
	TripodHead_Position_pid.i=data;
	printf("i= %f\r\n",TripodHead_Position_pid.i);
}
SHELL_EXPORT_CMD_AGENCY(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
ki, setki, test,SHELL_PARAM_FLOAT(p1));

void setkd( float data)
{
	TripodHead_Position_pid.d=data;
	printf("d= %f\r\n",TripodHead_Position_pid.d);
}
SHELL_EXPORT_CMD_AGENCY(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
kd, setkd, test,SHELL_PARAM_FLOAT(p1));

void sett( float data)
{
	target=data;
	printf("target= %f\r\n",target);
}
SHELL_EXPORT_CMD_AGENCY(SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
t, sett, test,SHELL_PARAM_FLOAT(p1));





