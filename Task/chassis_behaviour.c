#include "chassis_behaviour.h"


#define CHASSIS_OPEN_RC_SCALE 10 // in CHASSIS_OPEN mode, multiply the value. 在chassis_open 模型下，遥控器乘以该比例发送到can上


/**
  * @brief          通过判断角速度来判断云台是否到达极限位置
  * @param          对应轴的角速度，单位rad/s
  * @param          计时时间，到达GIMBAL_CALI_STEP_TIME的时间后归零
  * @param          记录的角度 rad
  * @param          反馈的角度 rad
  * @param          记录的编码值 raw
  * @param          反馈的编码值 raw
  * @param          校准的步骤 完成一次 加一
  */
#define chassis_cali_gyro_judge(gyro, cmd_time, angle_set, angle, ecd_set, ecd, step) \
    {                                                                                \
        if ((gyro) < GIMBAL_CALI_GYRO_LIMIT)                                         \
        {                                                                            \
            (cmd_time)++;                                                            \
            if ((cmd_time) > GIMBAL_CALI_STEP_TIME)                                  \
            {                                                                        \
                (cmd_time) = 0;                                                      \
                (angle_set) = (angle);                                               \
                (ecd_set) = (ecd);                                                   \
                (step)++;                                                            \
            }                                                                        \
        }                                                                            \
    }



static void chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_can_set == NULL || vy_can_set == NULL || wz_can_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }
    *vx_can_set = 0.0f;
    *vy_can_set = 0.0f;
    *wz_can_set = 0.0f;
}
static void chassis_no_move_control(fp32 *Vx_Set, fp32 *Vy_Set, fp32 *Vw_Set, chassis_move_t *Chassis_Move_Rc_to_Vector)
{
    if (Vx_Set == NULL || Vy_Set == NULL || Vw_Set == NULL || Chassis_Move_Rc_to_Vector == NULL)
    {
        return;
    }



    if(Chassis_Move_Rc_to_Vector->Chassis_RC->ch6 == 1)
    {
        *Vx_Set = 0.0f;
        *Vy_Set = 0.0f;
        *Vw_Set = 0.0f;

    }
    if(Chassis_Move_Rc_to_Vector->Chassis_RC->ch6 == 3)
    {
        *Vx_Set =4000;
    }

    if(Chassis_Move_Rc_to_Vector->Chassis_RC->ch6 == 2)
    {
        *Vx_Set =8000;
    }
}

static void chassis_remote_move_control(fp32 *Vx_Set, fp32 *Vy_Set, fp32 *Vw_Set, chassis_move_t *Chassis_Move_Rc_to_Vector)
{
    
		
		if (Vx_Set == NULL || Vy_Set == NULL || Vw_Set == NULL || Chassis_Move_Rc_to_Vector == NULL)
    {
        return;
    }
		
		*Vx_Set = Chassis_Move_Rc_to_Vector->Chassis_RC->ch1*4.5;         //test
		*Vy_Set = Chassis_Move_Rc_to_Vector->Chassis_RC->ch2*4.5;         //test
//		*Vw_Set = Chassis_Move_Rc_to_Vector->Chassis_RC->ch3;         //test
		
    return;
		
}



static void chassis_local_move_control(fp32 *Vx_Set, fp32 *Vy_Set, fp32 *Vw_Set, chassis_move_t *Chassis_Move_Rc_to_Vector)
{
	if (Vx_Set == NULL || Vy_Set == NULL || Vw_Set == NULL || Chassis_Move_Rc_to_Vector == NULL)
    {
        return;
    }


		switch(Chassis_Move_Rc_to_Vector->Chassis_RC->ch5) 
		{
			case 1:
				*Vx_Set =0;
			  *Vy_Set =0;
			  *Vw_Set =0;
			
				break;
			case 2:
				*Vx_Set =1000;
			  *Vy_Set =0;
			  *Vw_Set =0;
			break;
			case 3:
				
				*Vx_Set =-100;
			  *Vy_Set =-100;
			  *Vw_Set =0;
			break;
			
		}

    return;
}
//留意，这个底盘行为模式变量
chassis_behaviour_mode_e Chassis_Behaviour_Mode = CHASSIS_ZERO_FORCE;
/**
  * @brief          通过逻辑判断，赋值"chassis_behaviour_mode"成哪种模式
  * @param[in]      chassis_move_mode: 底盘数据
  * @retval         none
  */
void chassis_behaviour_mode_set(chassis_move_t *Chassis_Move_Mode)
{
    if (Chassis_Move_Mode == NULL)
    {
        return;
    }

    //根据行为模式选择一个底盘控制模式
    //添加自己的逻辑判断进入新模式
		/*
		此处说明一下
		九路一档==底盘不上电
		九路而当==底盘不动
		九路三档==底盘遥控
		五路四档==底盘定位
		
		这里进行修改
		把五路的移动到九路2通道
		
		*/
    if(Chassis_Move_Mode->Chassis_RC->ch9 == 1)
    {
        Chassis_Behaviour_Mode = CHASSIS_ZERO_FORCE;
    }
//    else if(Chassis_Move_Mode->Chassis_RC->ch9 == 2)
//    {
//        Chassis_Behaviour_Mode = CHASSIS_NO_MOVE;
//    }
    else if(Chassis_Move_Mode->Chassis_RC->ch9 == 2)//注意哦
    {
        Chassis_Behaviour_Mode = CHASSIS_LOCATING_MOVE;
    }
    else if(Chassis_Move_Mode->Chassis_RC->ch9 ==3)
    {
        Chassis_Behaviour_Mode = CHASSIS_REMOTE_MOVE;
    }


    //根据行为模式选择一个底盘控制模式
    if (Chassis_Behaviour_Mode == CHASSIS_ZERO_FORCE) //悬空
    {
        Chassis_Move_Mode->Chassis_Mode = CHASSIS_ZERO_FORCE;
    }
    else if (Chassis_Behaviour_Mode == CHASSIS_NO_MOVE) //固定
    {
        Chassis_Move_Mode->Chassis_Mode = CHASSIS_NO_MOVE;
    }
    else if (Chassis_Behaviour_Mode == CHASSIS_LOCATING_MOVE) //定位
    {
        Chassis_Move_Mode->Chassis_Mode = CHASSIS_LOCATING_MOVE;
    }
    else if (Chassis_Behaviour_Mode == CHASSIS_REMOTE_MOVE) //遥控
    {
        Chassis_Move_Mode->Chassis_Mode = CHASSIS_REMOTE_MOVE;
    }

}

//	 CHASSIS_ZERO_FORCE,                   //底盘无力, 跟没上电那样
//  CHASSIS_NO_MOVE,                      //底盘保持不动
//	 CHASSIS_LOCATING_MOVE,                //底盘定位
//	 CHASSIS_REMOTE_MOVE                   //遥控控制
void chassis_behaviour_control_set(fp32 *Vx_Set, fp32 *Vy_Set, fp32 *Vw_Set, chassis_move_t *Chassis_Move_Rc_To_Vector)
{
    if (Vx_Set == NULL || Vy_Set == NULL || Vw_Set == NULL || Chassis_Move_Rc_To_Vector == NULL)
    {

        return;
    }
    //根据行为模式选择一个底盘控制模式

    if (Chassis_Behaviour_Mode == CHASSIS_ZERO_FORCE)  //底盘无力, 跟没上电那样
    {
        chassis_zero_force_control(Vx_Set, Vy_Set, Vw_Set, Chassis_Move_Rc_To_Vector);
    }
    else	 if (Chassis_Behaviour_Mode == CHASSIS_NO_MOVE)  //固定底盘 不移动
    {
        chassis_no_move_control(Vx_Set, Vy_Set, Vw_Set, Chassis_Move_Rc_To_Vector);
    }
    else if (Chassis_Behaviour_Mode == CHASSIS_LOCATING_MOVE)  //底盘定位，现在开始补充
    {
			
			chassis_local_move_control(Vx_Set, Vy_Set, Vw_Set, Chassis_Move_Rc_To_Vector);
    }
    else if (Chassis_Behaviour_Mode == CHASSIS_REMOTE_MOVE)
    {
        chassis_remote_move_control(Vx_Set, Vy_Set, Vw_Set, Chassis_Move_Rc_To_Vector);
    }
    //遥控器设置模式
    //当云台在某些模式下，像初始化， 底盘不动
//    if (gimbal_cmd_to_chassis_stop())
//    {
//        chassis_behaviour_mode = CHASSIS_NO_MOVE;
//    }
}


