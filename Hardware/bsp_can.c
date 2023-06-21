#include "can.h"
#include "bsp_can.h"
#include "stdio.h"

//can1的 8个电机
motor_measure_t motor_can1[8] = {0}; //底盘8个速度电机
//can2的 8个电机
motor_measure_t motor_can2[8] = {0}; //转型轮



//void get_motor_offset(motor_measure_t *ptr, CAN_HandleTypeDef* hcan);
void get_motor_offset(motor_measure_t *ptr, uint8_t Data[]);


/**
* @Func		my_can_filter_init]
* @Brief       CAN1和CAN2滤波器配置
* @param       _hcan
* @Data        2022年12月16日
*/
void my_can_filter_init_recv_all(CAN_HandleTypeDef *_hcan) {
    //can1 &can2 use same filter config
    CAN_FilterTypeDef CAN_FilterType = {0};
    /*
    static CanTxMsgTypeDef		Tx1Message;
    static CanRxMsgTypeDef 		Rx1Message;
    */
    CAN_FilterType.FilterBank = 0;
    CAN_FilterType.FilterIdHigh = 0x0000;
    CAN_FilterType.FilterIdLow = 0x0000;
    CAN_FilterType.FilterMaskIdHigh = 0x0000; //(((uint32_t)0x1313<<3)&0xFFFF0000)>>16;
    CAN_FilterType.FilterMaskIdLow = 0x0000;//(((uint32_t)0x1313<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
    CAN_FilterType.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN_FilterType.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterType.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterType.FilterActivation = ENABLE;
    CAN_FilterType.SlaveStartFilterBank = 14;
    if (HAL_CAN_ConfigFilter(_hcan, &CAN_FilterType) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_Start(_hcan) != HAL_OK) {
        Error_Handler();
    }

}

void can_filter_recv_special(CAN_HandleTypeDef *_hcan) {
    CAN_FilterTypeDef CAN_FilterType;
    CAN_FilterType.FilterBank = 14;
    CAN_FilterType.FilterIdHigh = 0x0000;
    CAN_FilterType.FilterIdLow = 0x0000;
    CAN_FilterType.FilterMaskIdHigh = 0x0000; //(((uint32_t)0x1313<<3)&0xFFFF0000)>>16;
    CAN_FilterType.FilterMaskIdLow = 0x0000;  //(((uint32_t)0x1313<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
    CAN_FilterType.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN_FilterType.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterType.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterType.FilterActivation = ENABLE;
    CAN_FilterType.SlaveStartFilterBank = 14;
    if (HAL_CAN_ConfigFilter(_hcan, &CAN_FilterType) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_Start(_hcan) != HAL_OK) {
        Error_Handler();
    }
}

/*******************************************************************************************
  * @Func			void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* _hcan)
  * @Brief    HAL库中标准的CAN接收完成回调函数，需要在此处理通过CAN总线接收到的数据
  * @Param
  * @Retval		None
  * @Date     2015/11/24
 *******************************************************************************************/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *_hcan) {
    CAN_RxHeaderTypeDef CAN_RxHeader;
    uint8_t Rx_Data[8];
    uint8_t i;
    HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &CAN_RxHeader, Rx_Data);
    /********  CAN1 *******/
    if (_hcan->Instance == CAN1)
    {
        switch (CAN_RxHeader.StdId)
        {
        case CAN_Motor1_ID:
        case CAN_Motor2_ID:
        case CAN_Motor3_ID:
        case CAN_Motor4_ID:
        case CAN_Motor5_ID:
        case CAN_Motor6_ID:
        case CAN_Motor7_ID:
        case CAN_Motor8_ID:
        {
            i = CAN_RxHeader.StdId - CAN_Motor1_ID;
            if(motor_can1[i].msg_cnt<=50)  //上电后接收50次矫正 50次之后正常接收数据
            {
                motor_can1[i].msg_cnt++;
                get_motor_offset(&motor_can1[i], Rx_Data);
            }
            else get_motor_measure(&motor_can1[i], Rx_Data);
        }
        break;
        }
    }
    /********  CAN1 *******/
    //else if (_hcan->Instance == CAN2)
    else {
        switch (CAN_RxHeader.StdId)
        {
        case CAN_Motor1_ID:
        case CAN_Motor2_ID:
        case CAN_Motor3_ID:
        case CAN_Motor4_ID:
        case CAN_Motor5_ID:
        case CAN_Motor6_ID:
        case CAN_Motor7_ID:
        case CAN_Motor8_ID:
        {
            i = CAN_RxHeader.StdId - CAN_Motor1_ID;
            if(motor_can2[i].msg_cnt<=50)
            {
                motor_can2[i].msg_cnt++;
                get_motor_offset(&motor_can2[i], Rx_Data);
            }

            else get_motor_measure(&motor_can2[i], Rx_Data);
        }
        break;
        }
    }


}

int Can_TxMessage(CAN_HandleTypeDef *_hcan, uint8_t ide, uint32_t id, uint8_t len, uint8_t *data) {
    uint32_t TxMailbox;
    CAN_TxHeaderTypeDef CAN_TxHeader;
    HAL_StatusTypeDef HAL_RetVal;
    uint16_t i = 0;
    if (ide == 0) {
        CAN_TxHeader.IDE = CAN_ID_STD;    //标准帧
        CAN_TxHeader.StdId = id;
    } else {
        CAN_TxHeader.IDE = CAN_ID_EXT;    //扩展帧
        CAN_TxHeader.ExtId = id;
    }
    CAN_TxHeader.DLC = len;
    CAN_TxHeader.RTR = CAN_RTR_DATA;//数据帧,CAN_RTR_REMxx	OTE遥控帧
    CAN_TxHeader.TransmitGlobalTime = DISABLE;
    while (HAL_CAN_GetTxMailboxesFreeLevel(_hcan) == 0) {
        i++;
        if (i > 0xfffe)
            return 1;
    }
    HAL_RetVal = HAL_CAN_AddTxMessage(_hcan, &CAN_TxHeader, data, &TxMailbox);
    if (HAL_RetVal != HAL_OK)
        return 1;
    return 0;
}

/**
    Data[0] 转子机械角度高8位
    Data[1] 转子机械角度低8位
    Data[2] 转子转速高8位
    Data[3] 转子转速低8位
    Data[4] 实际转矩电流高8位
    Data[5] 实际转矩电流低8位
    Data[6] 电机温度
    Data[7] 空Null

    发送频率 1KHz
    转子机械角度 0 ~ 8191 (对应转子角度0~360)
    转子转速单位 RPM
    电机温度单位 °C

last_angle  上次角度更新
angle       转子机械角度高8位和第八位
speed_rpm   转子转速高8位和低八位
real_current实际输出转矩高8位和低8位
temperature 温度
total_angle 总角度
 */
void get_motor_measure(motor_measure_t *ptr, uint8_t Data[]) {
    ptr->last_angle = ptr->angle;                               //上次角度更新
    ptr->angle = (uint16_t) (Data[0] << 8 | Data[1]);           //转子机械角度高8位和第八位
    ptr->speed_rpm = (int16_t) (Data[2] << 8 | Data[3]);        //转子转速高8位和低八位
    ptr->real_current = (int16_t) (Data[4] << 8 | Data[5]);     //实际输出转矩高8位和低8位
    ptr->temperature = Data[6];                                 //温度     //Null

    if (ptr->angle - ptr->last_angle > 4096)
        ptr->round_cnt--;
    else if (ptr->angle - ptr->last_angle < -4096)
        ptr->round_cnt++;
    ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

void get_motor_offset(motor_measure_t *ptr, uint8_t Rx_Data[]) {
    ptr->angle = (uint16_t) (Rx_Data[0] << 8 | Rx_Data[1]);
    ptr->offset_angle = ptr->angle;
}

#define ABS(x)    ( (x>0) ? (x) : (-x) )

/**
  * @Func			get_total_angle(motor_measure_t *p)
  * @Brief    电机上电角度=0， 之后用这个函数  更新电机的相对开机后（为0）的相对角度。
  * @Param		
  * @Retval		None
  * @Date     2023/6/20
*/
int32_t get_total_angle(motor_measure_t *p) {
    int res1, res2, delta;
    if (p->angle < p->last_angle) {            //可能的情况
        res1 = p->angle + 8192 - p->last_angle;    //正转，delta=+
        res2 = p->angle - p->last_angle;                //反转	delta=-
    } else {    //angle > last
        res1 = p->angle - 8192 - p->last_angle;//反转	delta -
        res2 = p->angle - p->last_angle;                //正转	delta +
    }
		
	//	=DECE_RATIO_ER_TR*8191*36*Read_init_AS5048A[2]/360.0f;
		
		
		
		
		
		
    //不管正反转，肯定是转的角度小的那个是真的
    if (ABS(res1) < ABS(res2))
        delta = res1;
    else
        delta = res2;

    p->total_angle += delta;
    p->last_angle = p->angle;
		
    return p->total_angle;
		
		
		
		
}
/**
2006 C610   1:36
用于向电调发送控制指令控制电调的电流输出，
两个标识符（0x200和0x1FF)各自对应控制4个ID的电调。
控制转矩电流值范围-10000~0~10000，
对应电调输出的转矩电流范围-10~0~10A。
接收频率: 1KHz
转子机械角度值范围:0~8191(对应转子机械角度为0~360° )
转子转速值的单位为:RPM   8191 * 36 = 294876
电机温度的单位为:℃

3508 C620  1:19
用于向电调发送控制指令控制电调的电流输出，
两个标识符（0x200和0x1FF)各自对应控制4个ID的电调。
控制转矩电流值范围-16384~0~16384，
对应电调输出的转矩电流范围-20~0~20A。
接收频率: 1KHz
转子机械角度值范围:0~8191(对应转子机械角度为0~360° )   8191*19 =155629
转子转速值的单位为:RPM
电机温度的单位为:℃


*/
void set_motor_A(CAN_HandleTypeDef *_hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4) {

    uint8_t data[8] = {0};
    data[0] = iq1 >> 8;
    data[1] = iq1;
    data[2] = iq2 >> 8;
    data[3] = iq2;
    data[4] = iq3 >> 8;
    data[5] = iq3;
    data[6] = iq4 >> 8;
    data[7] = iq4;
//    hcan->pTxMsg->StdId = 0x200;
//    hcan->pTxMsg->IDE = CAN_ID_STD;
//    hcan->pTxMsg->RTR = CAN_RTR_DATA;
//    hcan->pTxMsg->DLC = 0x08;
    Can_TxMessage(_hcan, 0, 0x200, 8, (uint8_t *) data); //0代表标准正

}

void set_motor_B(CAN_HandleTypeDef *_hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4) //4个电机转向
{
    uint8_t data[8] = {0};
    data[0] = iq1 >> 8;
    data[1] = iq1;
    data[2] = iq2 >> 8;
    data[3] = iq2;
    data[4] = iq3 >> 8;
    data[5] = iq3;
    data[6] = iq4 >> 8;
    data[7] = iq4;

    Can_TxMessage(_hcan, 0, 0x1FF, 8, (uint8_t *) data); //0代表标准正
}

void set_motor(CAN_HandleTypeDef *_hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4,
               int16_t iq5, int16_t iq6, int16_t iq7, int16_t iq8) 
{
    set_motor_A(_hcan, iq1, iq2, iq3, iq4);
    set_motor_B(_hcan, iq5, iq6, iq7, iq8);
}
////can1的 8个电机
//motor_measure_t motor_can1[8] = {0}; //底盘6个速度电机
////can2的 8个电机
//motor_measure_t motor_can2[8] = {0}; //转型轮

//typedef struct {
//    int16_t speed_rpm;      //转速
//    float real_current;     //实际输出转矩
//    int16_t given_current;  //扭矩
//    uint8_t temperature;    //温度
//    uint16_t angle;         //abs angle range:[0,8191] 角度范围[0,8191]
//    uint16_t last_angle;    //abs angle range:[0,8191]
//    uint16_t offset_angle;  //偏差
//    int32_t round_cnt;      //电机转的圈数
//    int32_t total_angle;    //当前总角度 圈数
//    u8 buf_idx;
//    u16 angle_buf[FILTER_BUF_LEN];
//    u16 fited_angle;        //修正角度？
//    u32 msg_cnt;            //???
//} motor_measure_t;
/**
  * @brief          返回底盘电机 3508电机数据指针
  * @param[in]      i: 电机编号,范围[0,7]
  * @retval         电机数据指针
  */
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_can1[(i & 0x07)];
}

const motor_measure_t *get_chassis_motor2_measure_point(uint8_t i)
{
    return &motor_can2[(i & 0x07)];
}
