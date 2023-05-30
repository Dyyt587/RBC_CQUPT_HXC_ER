#include "task.h"
#include "stdio.h"
#define int_abs(x) ((x) > 0 ? (x) : (-x))

//typedef enum
//{
//    ROBOT_INIT,                 //初始化
//    ROBOT_LOCATE_POSTION_A,     //定位到圆环A区
//    ROBOT_CORRENT_POSTION_A,    //矫正位置圆环A区
//    ROBOT_LOCATE_RINGREGION_A,  //到达取圆环A区
//    ROBOT_FENTCH_RING,          //取环
//	   ROBOT_SHOOT_RING,           //射环
//    ROBOT_STOP
//} robot_behaviour_e;




robot_behaviour_e  Robot_Behaviours = ROBOT_INIT;      //当前循环结束的(状态机的)状态
robot_behaviour_e  Robot_nowBehaviours = ROBOT_INIT;   //当前状态
robot_behaviour_e  Robot_lastBehaviours = ROBOT_INIT;  //上次状态(用于记录前一状态以区分状态的来源)

int  g_WorkLoopCnt=0;

robot_behaviour_e robot_init(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_INIT;


    if(1)   //初始化接结束 进入下一状态
    {

        nextBehaviours = ROBOT_LOCATE_POSTION_A;

    }

    return nextBehaviours;
}


robot_behaviour_e robot_locate_postion_a(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_LOCATE_POSTION_A;
    if(1)
    {

        nextBehaviours = ROBOT_CORRENT_POSTION_A;
    }
    return nextBehaviours;
}


robot_behaviour_e robot_corrent_postion_a(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_CORRENT_POSTION_A;
    if(1)
    {

        nextBehaviours = ROBOT_LOCATE_RINGREGION_A;
    }
    return nextBehaviours;
}

robot_behaviour_e robot_locate_ringregion_a(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_LOCATE_RINGREGION_A;
    if(1)
    {

        nextBehaviours = ROBOT_FENTCH_RING;
    }
    return nextBehaviours;
}


robot_behaviour_e robot_fentch_ring(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_FENTCH_RING;
    if(1)
    {

        nextBehaviours = ROBOT_SHOOT_RING;
    }
    return nextBehaviours;
}


robot_behaviour_e robot_shoot_ring(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_SHOOT_RING;
    if(1)
    {

         nextBehaviours = ROBOT_CORRENT_POSTION_A;
    }
    return nextBehaviours;
}


//    ROBOT_INIT,                 //初始化
//    ROBOT_LOCATE_POSTION_A,     //定位到圆环A区
//    ROBOT_CORRENT_POSTION_A,    //矫正位置圆环A区
//    ROBOT_LOCATE_RINGREGION_A,  //到达取圆环A区
//    ROBOT_FENTCH_RING,          //取环
//	   ROBOT_SHOOT_RING,           //射环
//    ROBOT_STOP

robot_behaviour_e robot_stop(void)
{
    robot_behaviour_e nextBehaviours = ROBOT_STOP;

    switch (Robot_lastBehaviours)
    {
    case ROBOT_INIT:
        nextBehaviours = ROBOT_INIT;
        printf("resume...\r\n");
        break;
    case ROBOT_LOCATE_POSTION_A:
        nextBehaviours = ROBOT_LOCATE_POSTION_A;
        printf("resume...\r\n");
        break;
    case ROBOT_CORRENT_POSTION_A:
        nextBehaviours = ROBOT_CORRENT_POSTION_A;
        printf("resume...\r\n");
        break;
    case ROBOT_LOCATE_RINGREGION_A:
        nextBehaviours = ROBOT_LOCATE_RINGREGION_A;
        printf("resume...\r\n");
        break;
    case ROBOT_FENTCH_RING:
        nextBehaviours = ROBOT_FENTCH_RING;
        printf("resume...\r\n");
        break;
    case ROBOT_SHOOT_RING:
        nextBehaviours = ROBOT_SHOOT_RING;
        printf("resume...\r\n");
        break;
    default:
        break;
    }

		return nextBehaviours;
}












void robot_run_loop()
{
    switch(Robot_Behaviours)
    {
    /*开机初始化自检*/
    case ROBOT_INIT:
        //	g_washerStatus = washer_do_init();
        Robot_Behaviours=  robot_init();
        break;

    /*定位到位置A*/
    case ROBOT_LOCATE_POSTION_A:
        Robot_Behaviours=  robot_locate_postion_a();
        break;

    /*矫正位置A坐标*/
    case ROBOT_CORRENT_POSTION_A:
        Robot_Behaviours=  robot_corrent_postion_a();
        break;

    /*到达取环区*/
    case ROBOT_LOCATE_RINGREGION_A:
        Robot_Behaviours=  robot_locate_ringregion_a();
        break;
    /*取环*/
    case ROBOT_FENTCH_RING:
        Robot_Behaviours=  robot_fentch_ring();
        break;
    /*射环*/
    case ROBOT_SHOOT_RING:
        Robot_Behaviours=  robot_shoot_ring();
        break;
    /*停*/
    case ROBOT_STOP:
        Robot_Behaviours=  robot_stop();
        break;
    default:
        break;
    }

    if (Robot_Behaviours != Robot_nowBehaviours)
    {
        Robot_lastBehaviours = Robot_nowBehaviours;
        Robot_nowBehaviours = Robot_Behaviours;
    }
}

void yaokong(void)
{
	
}
