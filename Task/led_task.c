#include "led_task.h"
#include "cmsis_os.h"
#include "action_usart.h"
#include "main.h"
#include "bsp_sbus.h"

#define Print_remote  		0			 	//打印遥控器数据
#define Print_realAngle  	0   		//真实角度,用于归零
#define Print_Angle  			0			 	//轮子角度
#define Print_Speed				0			 	//轮子速度 
#define Print_Coordinate		0			 	//坐标信息 


//遥控器
extern rc_info_t rc;
//全场定位
int testcount=0;
extern float pos_x;//坐标X--ZBx
extern float pos_y;//坐标Y--ZBy
extern float zangle;//航向角
extern float xangle;//俯仰角
extern float yangle;//横滚角
extern float w_z;//航向角速

extern float set_pos_x;
extern float set_pos_y;
extern float set_zangle;
extern int move_flag;
int Limit2(int x,int max)
{
    if(x>max) x=max;
    if(x<-max) x=-max;
    return x;
}



void LED_Delay(uint32_t time)
{
    //HAL_Delay(time);
    osDelay(time);
}
void LED_A(uint8_t flag)
{
    if(flag)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
}
void LED_B(uint8_t flag)
{
    if(flag)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET);
}
void LED_ALL_OFF()
{
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8,GPIO_PIN_SET);
}
void RUN_LED(void)
{
    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
    LED_Delay(100);

    LED_ALL_OFF();
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8,GPIO_PIN_RESET);
    LED_Delay(100);

}

/**
  * @brief          led RGB任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void led_task(void const * argument)
{
	while(1){
		
	  RUN_LED();
		#if   Print_remote
        printf("ch1:%4d ch2:%4d ch3:%4d ch4:%4d ch5:%4d ch6:%4d \r\n",rc.ch1,rc.ch2,rc.ch3,rc.ch4,rc.ch5,rc.ch6);
#endif
        /*******轮子速度*******/
#if   Print_Speed
//		 printf("1:%6d,2:%6d,3:%6d,4:%6d,5:%6d,6:%6d\n",
//               motor_can1[0].speed_rpm,motor_can1[1].speed_rpm,motor_can1[2].speed_rpm,
//               motor_can1[3].speed_rpm,motor_can1[4].speed_rpm,motor_can1[5].speed_rpm);

        printf("%f\r\n",(float)motor_can1[0].speed_rpm);
#endif


//extern float pos_x;//坐标X--ZBx
//extern float pos_y;//坐标Y--ZBy
//extern float zangle;//航向角
//extern float xangle;//俯仰角
//extern float yangle;//横滚角
//extern float w_z;//航向角速

//#if   Print_Coordinate
//        printf("x:%.2f	y:%.2f	yaw:%.2f		pitch:%.2f	roll:%.2f	w:%.2f \r\n",
//               pos_x,pos_y,
//               zangle,xangle,yangle,
//               w_z);
//#endif


//        if(rc.ch6==1)
//        {
//            switch(rc.ch5 )
//            {
//            case 1:
//                printf("ch1:%4d ch2:%4d ch3:%4d ch4:%4d ch5:%4d ch6:%4d \r\n",rc.ch1,rc.ch2,rc.ch3,rc.ch4,rc.ch5,rc.ch6);
//                break;
//            case 2:
//                printf("1=%6d,2=%6d,3=%6d,4=%6d,5=%6d,%6d\r\n",
//                       motor_can1[0].speed_rpm,motor_can1[1].speed_rpm,motor_can1[2].speed_rpm,
//                       motor_can1[3].speed_rpm,motor_can1[4].speed_rpm,motor_can1[5].speed_rpm);
//                break;
//            case 3:
//                printf("x:%.2f	y:%.2f	yaw:%.2f		pitch:%.2f	roll:%.2f	w:%.2f \r\n",
//                       pos_x,pos_y,
//                       zangle,xangle,yangle,
//                       w_z);
//                break;
//            default :
//                break;
//            }
//        }
//        else
//        {
//        }





//        Usart2Printf("n0.val=%d\xff\xff\xff",(int)pos_y);
//        Usart2Printf("n1.val=%d\xff\xff\xff",(int)pos_x);
//        Usart2Printf("n2.val=%d\xff\xff\xff",(int)zangle);
//        Usart2Printf("n3.val=%d\xff\xff\xff",(int)w_z*10);

//        Usart2Printf("n4.val=%d\xff\xff\xff",rc.ch1);
//        Usart2Printf("n5.val=%d\xff\xff\xff",rc.ch2);
//        Usart2Printf("n6.val=%d\xff\xff\xff",rc.ch3);
//        Usart2Printf("n7.val=%d\xff\xff\xff",rc.ch4);
//        Usart2Printf("n8.val=%d\xff\xff\xff",rc.ch5);
//        Usart2Printf("n9.val=%d\xff\xff\xff",rc.ch6);

//        Usart2Printf("n10.val=%d\xff\xff\xff",motor_can1[0].speed_rpm);
//        Usart2Printf("n11.val=%d\xff\xff\xff",motor_can1[1].speed_rpm);
//        Usart2Printf("n12.val=%d\xff\xff\xff",motor_can1[2].speed_rpm);
//        Usart2Printf("n13.val=%d\xff\xff\xff",motor_can1[3].speed_rpm);
//        Usart2Printf("n14.val=%d\xff\xff\xff",motor_can1[4].speed_rpm);
//        Usart2Printf("n15.val=%d\xff\xff\xff",motor_can1[5].speed_rpm);

        Usart2Printf("n16.val=%d\xff\xff\xff",(int)pos_y*2);
        Usart2Printf("n17.val=%d\xff\xff\xff",(int)pos_x*3);
        Usart2Printf("n18.val=%d\xff\xff\xff",(int)zangle);
        Usart2Printf("n19.val=%d\xff\xff\xff",(int)w_z*10*4);

		
		
		
		
		
		
		
		
		
		
		osDelay(5);
		
	}

}
