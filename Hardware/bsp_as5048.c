#include "bsp_as5048.h"
#include "spi.h"
#include "stdio.h"
#include "chassis_task.h"
#include "chassis_task.h"
#define AS5048_SPI       hspi4 /* for dji remote controler reciever */


#define __Read_NOP 0xc000
#define __Read_Clear_Error_Flag 0x4001
#define __Read_Angle     0xFFFF //��ȡ�Ƕ�����
////������1//
//#define __AS5048A1_CS_ENABLE()        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET)
//#define __AS5048A1_CS_DISABLE()       HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)//Ƭѡ1��SPIʹ�ɶ�

//������1//
#define __AS5048A_CS_GPIO_Port_1			  GPIOF
#define __AS5048A_CS__Pin_1              GPIO_PIN_1
//������2
#define __AS5048A_CS_GPIO_Port_2			  GPIOE
#define __AS5048A_CS__Pin_2              GPIO_PIN_4
//������3
#define __AS5048A_CS_GPIO_Port_3			  GPIOF
#define __AS5048A_CS__Pin_3              GPIO_PIN_0
//������4
#define __AS5048A_CS_GPIO_Port_4			  GPIOC
#define __AS5048A_CS__Pin_4              GPIO_PIN_2


//������1
#define __AS5048A_CS_1_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_1, __AS5048A_CS__Pin_1, GPIO_PIN_RESET)
#define __AS5048A_CS_1_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_1, __AS5048A_CS__Pin_1, GPIO_PIN_SET)//Ƭѡ1��SPIʹ�ɶ�
//������2
#define __AS5048A_CS_2_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_2, __AS5048A_CS__Pin_2, GPIO_PIN_RESET)
#define __AS5048A_CS_2_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_2, __AS5048A_CS__Pin_2, GPIO_PIN_SET)//Ƭѡ2��SPIʹ�ɶ�
//������3
#define __AS5048A_CS_3_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_3, __AS5048A_CS__Pin_3, GPIO_PIN_RESET)
#define __AS5048A_CS_3_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_3, __AS5048A_CS__Pin_3, GPIO_PIN_SET)//Ƭѡ3��SPIʹ�ɶ�
//������4
#define __AS5048A_CS_4_ENABLE()        HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_4, __AS5048A_CS__Pin_4, GPIO_PIN_RESET)
#define __AS5048A_CS_4_DISABLE()       HAL_GPIO_WritePin(__AS5048A_CS_GPIO_Port_4, __AS5048A_CS__Pin_4, GPIO_PIN_SET)//Ƭѡ4��SPIʹ�ɶ�



//������1
#define __AS5048A1_CS_ENABLE()        HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET)
#define __AS5048A1_CS_DISABLE()       HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET)//Ƭѡ1��SPIʹ�ɶ�
//������2
#define __AS5048A2_CS_ENABLE()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_RESET)
#define __AS5048A2_CS_DISABLE()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET)//Ƭѡ1��SPIʹ�ɶ�
//������3
#define __AS5048A3_CS_ENABLE()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET)
#define __AS5048A3_CS_DISABLE()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET)//Ƭѡ1��SPIʹ�ɶ�
//������4
#define __AS5048A4_CS_ENABLE()        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET)
#define __AS5048A4_CS_DISABLE()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET)//Ƭѡ1��SPIʹ�ɶ�




uint16_t command;
uint16_t angleValue;

extern float Read_init_AS5048A[4];

uint16_t SPI_AS5048A_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A2_CS_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A2_CS_DISABLE();
    __AS5048A2_CS_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A2_CS_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}


float inline GET_AS5048A_ANGLE(uint16_t data)
{
    float angel;
    angel = (data*360.0)/16384;
    return angel;
}


uint16_t SPI_AS5048A_1_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_1_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_1_DISABLE();
    __AS5048A_CS_1_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_1_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

uint16_t SPI_AS5048A_2_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_2_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_2_DISABLE();
    __AS5048A_CS_2_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_2_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

uint16_t SPI_AS5048A_3_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_3_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_3_DISABLE();
    __AS5048A_CS_3_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_3_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}

uint16_t SPI_AS5048A_4_ReadData(void)
{
    uint16_t angle_value;
    command = __Read_Angle;
    __AS5048A_CS_4_ENABLE() ;
    HAL_SPI_Transmit(&AS5048_SPI,(unsigned char *)&command,1,100);
    __AS5048A_CS_4_DISABLE();
    __AS5048A_CS_4_ENABLE() ;
    HAL_SPI_TransmitReceive(&AS5048_SPI,(unsigned char *)&command,(unsigned char *)&angleValue,1,100 );
    __AS5048A_CS_4_DISABLE();
    angle_value = angleValue & 0x3FFF;
    return angle_value;
}


/**
ԭ����test_task�����õĺ�����Ŀ���Ǵ�ӡ�����ڼ���getֵ
�����������ڴ�ӡ��ʼ���ĸ���ֵ�������Լ�����������/���ע��
������ѭ��������ִ��
**/
void Test_AS5048(void)
{
	
	uint8_t i = 0;
//	Read_init_AS5048A[0]=GET_AS5048A_ANGLE(SPI_AS5048A_1_ReadData());
//	Read_init_AS5048A[1]=GET_AS5048A_ANGLE(SPI_AS5048A_2_ReadData());
//	Read_init_AS5048A[2]=GET_AS5048A_ANGLE(SPI_AS5048A_3_ReadData());
//	Read_init_AS5048A[3]=GET_AS5048A_ANGLE(SPI_AS5048A_4_ReadData());
	

	//�˼�Ϊʵʱ�õ��Ŵ�������ֵ
//    printf("1:%7.3f  2:%7.3f 3:%7.3f  4:%7.3f\r\n",
//           GET_AS5048A_ANGLE(SPI_AS5048A_1_ReadData()),
//           GET_AS5048A_ANGLE(SPI_AS5048A_2_ReadData()),
//           GET_AS5048A_ANGLE(SPI_AS5048A_3_ReadData()),
//           GET_AS5048A_ANGLE(SPI_AS5048A_4_ReadData())
//          );
	
	
	//��Ϊһֱ���ͳ�ʼ����ֵ��ûɶ�ã�
//	printf("1:%f  2:%f 3:%f  4:%f\r\n",
//	Init_test_AS5048_test(0),
//	Init_test_AS5048_test(1),
//  Init_test_AS5048_test(2),
//  Init_test_AS5048_test(3));


//	printf("a:%f  b:%f c:%f  d:%f\r\n",
//	GET_AS5048A_ANGLE(SPI_AS5048A_1_ReadData()),
//  GET_AS5048A_ANGLE(SPI_AS5048A_2_ReadData()),
//	GET_AS5048A_ANGLE(SPI_AS5048A_3_ReadData()),
//	GET_AS5048A_ANGLE(SPI_AS5048A_4_ReadData()));
	
//	for(i=0;i<4;i++)
//	{
//		chassis_move_t->Wheel_Dir[i].angle=12;
//	}
//	
	
	


} 
void Init_test_AS5048(void)//�������ڳ�ʼ����ִֻ��һ��
{
		printf("a:%f  b:%f c:%f  d:%f\r\n",//�õ��ʼ�Ĵ�������ֵ
	GET_AS5048A_ANGLE(SPI_AS5048A_1_ReadData()),
  GET_AS5048A_ANGLE(SPI_AS5048A_2_ReadData()),
	GET_AS5048A_ANGLE(SPI_AS5048A_3_ReadData()),
	GET_AS5048A_ANGLE(SPI_AS5048A_4_ReadData()));
	//������Read_init_AS5048A===GET_AS5048A_ANGLE������
	//����������ʵ��������ȫ��ͬ�Ķ���
	//Ϊ�������READ���б仯����
		Read_init_AS5048A[0]=GET_AS5048A_ANGLE(SPI_AS5048A_1_ReadData());
    Read_init_AS5048A[1]=GET_AS5048A_ANGLE(SPI_AS5048A_2_ReadData())-22.395996-206.976563 ;//-246.0
    Read_init_AS5048A[2]=GET_AS5048A_ANGLE(SPI_AS5048A_3_ReadData());//-160.410645-2.912109
    Read_init_AS5048A[3]=GET_AS5048A_ANGLE(SPI_AS5048A_4_ReadData())+207.680176-247.912109;//-248.0
	
//	for(int i=0;i<4;i++)
//	{
//		 if(Read_init_AS5048A[i]>=180)
//			 Read_init_AS5048A[i]-=360;
//	}
	//�˴��������READ����һ����
	//��ֹ��������
//	printf("1:%f  2:%f 3:%f  4:%f\r\n",
//	Init_test_AS5048_test(0),
//	Init_test_AS5048_test(1),
//	Init_test_AS5048_test(2),
//	Init_test_AS5048_test(3));
	
	//�˴�������Ϊ�˼�����Ƿ�������������һЩ�����ʼֵ
	
//	Read_init_AS5048A[0]= 45.0;
//  Read_init_AS5048A[1]= 45.0;
//  Read_init_AS5048A[2]= -45.0;
//  Read_init_AS5048A[3]= -45.0;

}

//
float Init_test_AS5048_test(int index)
{
	if(index>3 || index<0)while(1);
	return Read_init_AS5048A[index];
}
