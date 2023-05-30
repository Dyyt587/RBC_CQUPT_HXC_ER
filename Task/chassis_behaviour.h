#ifndef __CHASSIS_BEHAVIOUR_H
#define __CHASSIS_BEHAVIOUR_H
#include "chassis_task.h"
//#include "motor.h"
#include "pid.h"
#include "bsp_sbus.h"
#include "bsp_can.h"
typedef float fp32;
typedef double fp64;




typedef struct
{
    int id;
    pid_t pid_pos;      //每个舵轮转向的位置环
    pid_t pid_speed;    //每个舵轮的速度环
    float speed;        //运行速度
    float speed_set;    //运行速度
    int16_t give_current; //赋值电流值
    float rpm;          //转多少圈
} wheel_speed_t;

typedef struct
{
    int id;
    pid_t pid_pos;      //每个舵轮转向的位置环
    pid_t pid_speed;    //每个舵轮的速度环
    float speed;        //运行速度
    float speed_set;    //运行速度
    float angle;        //当前角度
    float angle_set;    //设定角度
    int16_t give_current; //赋值电流值

    float rpm;          //转多少圈

} wheel_dir_t;




typedef enum
{
    CHASSIS_ZERO_FORCE,                   //底盘无力, 跟没上电那样
    CHASSIS_NO_MOVE,                      //底盘保持不动
    CHASSIS_LOCATING_MOVE,                //底盘定位
    CHASSIS_REMOTE_MOVE                   //遥控控制
} chassis_behaviour_mode_e;

/*******底盘模型*********/
typedef enum
{
    Omni3_Car,
    Omni4_Car,
    Wheel3_Car,
    Wheel4_Car
} chassis_kinematics_mode_e;

typedef struct
{

//  const gimbal_motor_t *chassis_yaw_motor;   //will use the relative angle of yaw gimbal motor to calculate the euler angle.底盘使用到yaw云台电机的相对角度来计算底盘的欧拉角.
//  const gimbal_motor_t *chassis_pitch_motor; //will use the relative angle of pitch gimbal motor to calculate the euler angle.底盘使用到pitch云台电机的相对角度来计算底盘的欧拉角
//  const fp32 *chassis_INS_angle;             //the point to the euler angle of gyro sensor.获取陀螺仪解算出的欧拉角指针

//  chassis_mode_e last_chassis_mode;          //last state machine.底盘上次控制状态机
//  chassis_motor_t motor_chassis[4];          //chassis motor data.底盘电机数据
//  pid_type_def motor_speed_pid[4];             //motor speed PID.底盘电机速度pid
//  pid_type_def chassis_angle_pid;              //follow angle PID.底盘跟随角度pid

//  first_order_filter_type_t chassis_cmd_slow_set_vx;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
//  first_order_filter_type_t chassis_cmd_slow_set_vy;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值


    const rc_info_t *Chassis_RC;               //底盘使用的遥控器指针, the point to remote control
    const motor_measure_t *Chassis_Motor_Measure[8];
    chassis_behaviour_mode_e Chassis_Mode;               //state machine. 底盘控制状态机
    chassis_behaviour_mode_e Last_Chassis_Mode;          //last state machine.底盘上次控制状态机
    chassis_kinematics_mode_e Chassis_Kinematics_Mode;   //底盘运动学模型

    wheel_speed_t Wheel_Speed[4] ;				//驱动轮
    wheel_dir_t   Wheel_Dir[4]   ;           //方向轮
    pid_t chassis_angle_pid;
    fp32 Vx;                          //底盘速度 前进方向 前为正，单位 m/s
    fp32 Vy;                          //底盘速度 左右方向 左为正  单位 m/s
    fp32 Vw;                          //底盘旋转角速度，逆时针为正 单位 rad/s
    fp32 Vx_Set;                      //底盘设定速度 前进方向 前为正，单位 m/s
    fp32 Vy_Set;                      //底盘设定速度 左右方向 左为正，单位 m/s
    fp32 Vw_Set;                      //底盘设定旋转角速度，逆时针为正 单位 rad/s

    fp32 Vx_Max_Speed;   //前进方向最大速度 单位m/s
    fp32 Vx_Min_Speed;   //后退方向最大速度 单位m/s
    fp32 Vy_Max_Speed;   //左方向最大速度 单位m/s
    fp32 Vy_Min_Speed;   //右方向最大速度 单位m/s


    fp32 Chassis_Yaw;    //底盘yaw角度
    fp32 Chassis_Yaw_Set;//底盘设定yaw角度
    fp32 Postion_X;   //
    fp32 Postion_Y;
    fp32 Postion_X_Set;   //
    fp32 Postion_Y_Set;
} chassis_move_t;



void chassis_behaviour_mode_set(chassis_move_t *Chassis_Move_Mode);
void chassis_behaviour_control_set(fp32 *Vx_Set, fp32 *Vy_Set, fp32 *Angle_Set, chassis_move_t *Chassis_Move_Rc_To_Vector);



#endif
