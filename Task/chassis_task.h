#ifndef __CHASSIS_TASK_H
#define __CHASSIS_TASK_H
#include "main.h"
#include "chassis_behaviour.h"

typedef float fp32;
typedef double fp64;
#define MAX_MOTOR_CAN_CURRENT 10000
/*底盘电机速度环PID*/
//驱动轮速度环
#define WHEEL_SPEED_PID_SPEED_KP 5.0f
#define WHEEL_SPEED_PID_SPEED_KI 0.4f
#define WHEEL_SPEED_PID_SPEED_KD 0.0f
#define WHEEL_SPEED_PID_SPEED_MAX_OUT 10000
#define WHEEL_SPEED_PID_SPEED_MAX_IOUT 2000
//转向轮角度环
#define WHEEL_DIR_PID_POSTION_KP 400.0f
#define WHEEL_DIR_PID_POSTION_KI 0.0f
#define WHEEL_DIR_PID_POSTION_KD 10.0f
#define WHEEL_DIR_PID_POSTION_MAX_OUT 10000
#define WHEEL_DIR_PID_POSTION_MAX_IOUT 2000
//转向轮速度环
#define WHEEL_DIR_PID_SPEED_KP 4.5f
#define WHEEL_DIR_PID_SPEED_KI 0.08f
#define WHEEL_DIR_PID_SPEED_KD 1.5f
#define WHEEL_DIR_PID_SPEED_MAX_OUT 10000
#define WHEEL_DIR_PID_SPEED_MAX_IOUT 2000

/*底盘任务间隔时间 ms */
#define CHASSIS_CONTROL_TIME_MS 2

extern void chassis_task(void const * argument);

#endif
