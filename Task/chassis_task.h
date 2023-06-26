#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H
#include "main.h"
#include "chassis_behaviour.h"


#define MAX_MOTOR_CAN_CURRENT 10000
/*���̵���ٶȻ�PID*/
//�������ٶȻ�
#define WHEEL_SPEED_PID_SPEED_KP 5.0f
#define WHEEL_SPEED_PID_SPEED_KI 0.4f
#define WHEEL_SPEED_PID_SPEED_KD 0.0f
#define WHEEL_SPEED_PID_SPEED_MAX_OUT 10000
#define WHEEL_SPEED_PID_SPEED_MAX_IOUT 2000
//ת���ֽǶȻ�
#define WHEEL_DIR_PID_POSTION_KP 400.0f
#define WHEEL_DIR_PID_POSTION_KI 0.0f
#define WHEEL_DIR_PID_POSTION_KD 10.0f
#define WHEEL_DIR_PID_POSTION_MAX_OUT 10000
#define WHEEL_DIR_PID_POSTION_MAX_IOUT 2000
//ת�����ٶȻ�
#define WHEEL_DIR_PID_SPEED_KP 4.5f
#define WHEEL_DIR_PID_SPEED_KI 0.08f
#define WHEEL_DIR_PID_SPEED_KD 1.5f
#define WHEEL_DIR_PID_SPEED_MAX_OUT 10000
#define WHEEL_DIR_PID_SPEED_MAX_IOUT 2000

/*����������ʱ�� ms */
#define CHASSIS_CONTROL_TIME_MS 2

extern void chassis_task(void const * argument);

/*���̳�ʼ����У��*/
//void chassis_location_init(chassis_move_t *Chassis_Move_Control_Loop);

//ȫ����λ
//void set_pos(float X_target, float Y_target, float Angle_target,
//             float X_current, float Y_current, float Angle_current,
//							 chassis_move_t *Chassis_Move_Control_Loop);
//void Set_speed(int Velocity_X, int Velocity_Y, int W,chassis_move_t *Chassis_Move_Control_Loop);

#endif
