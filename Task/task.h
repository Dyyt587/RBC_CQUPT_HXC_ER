#ifndef __TASK_H
#define __TASK_H


typedef enum
{
    ROBOT_INIT,                 //初始化
    ROBOT_LOCATE_POSTION_A,     //定位到圆环A区
    ROBOT_CORRENT_POSTION_A,    //矫正位置圆环A区
    ROBOT_LOCATE_RINGREGION_A,  //到达取圆环A区
    ROBOT_FENTCH_RING,          //取环
    ROBOT_SHOOT_RING,           //射环
    ROBOT_STOP
} robot_behaviour_e;





robot_behaviour_e robot_init(void);
robot_behaviour_e robot_locate_postion_a(void);
robot_behaviour_e robot_corrent_postion_a(void);
robot_behaviour_e robot_locate_ringregion_a(void);
robot_behaviour_e robot_fentch_ring(void);
robot_behaviour_e robot_shoot_ring(void);
robot_behaviour_e robot_stop(void);



#endif
