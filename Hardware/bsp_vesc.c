#include "bsp_vesc.h"
#include "datatypes.h"  
#include "bldc_interface.h"
#include "bldc_interface_uart.h"
#include "buffer.h"
#include "packet.h"
#include "crc.h"
#include "usart.h"
#include "stdlib.h"
#include "stdio.h"
#include "freertos.h"
#include "cmsis_os.h"

uint8_t rx_data_0=0;
uint8_t rx_data_1=0;
uint8_t is_send_uart7=1;
uint8_t is_send_uart8=0;

/**
*vesc底层发送函数，内部调用
*/
static void send_package_0(unsigned char  *s,unsigned int len)
{
  if(len>(PACKET_MAX_PL_LEN+5)){
    return;
  }
	if(is_send_uart7){
	HAL_UART_Transmit(&huart7,s,len,0xffffffff);
	}
	if(is_send_uart8){
		HAL_UART_Transmit(&huart8,s,len,0xffffffff);
	}

}

void bldc_val_received(mc_values *val)
{
	//需要判断当前在控制的电机
printf("\r\n");
printf("Input voltage: %.2f V\r\n", val->v_in);
printf("Temp:          %.2f degC\r\n", val->temp_mos);
printf("Current motor: %.2f A\r\n", val->current_motor);
printf("Current in:    %.2f A\r\n", val->current_in);
printf("RPM:           %.1f RPM\r\n", val->rpm);
printf("Duty cycle:    %.1f %%\r\n", val->duty_now * 100.0f);
printf("Ah Drawn:      %.4f Ah\r\n", val->amp_hours);
printf("Ah Regen:      %.4f Ah\r\n", val->amp_hours_charged);
printf("Wh Drawn:      %.4f Wh\r\n", val->watt_hours);
printf("Wh Regen:      %.4f Wh\r\n", val->watt_hours_charged);
printf("Tacho:         %i counts\r\n", val->tachometer);
printf("Tacho ABS:     %i counts\r\n", val->tachometer_abs);
printf("Fault Code:    %s\r\n", bldc_interface_fault_to_string(val->fault_code));
}

void vesc_init(void)
{
	
	bldc_interface_uart_init(send_package_0,0);//默认控制电机0
	bldc_interface_set_rx_value_func(bldc_val_received);//当收到数据包后就调用它给用户处理 
	HAL_UART_Receive_IT(&huart7,&rx_data_0,1);
	//HAL_UART_Receive_IT(&huart8,&rx_data_1,1);

//}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
	//迁移到shell_port.c中
//		bldc_interface_uart_process_byte(  rx_data );
//		HAL_UART_Receive_IT(&huart7,&rx_data,1);

}
//改变指向的电机
//实现：改变底层发送接收的函数指针
void vesc_set_point(uint8_t num)
{
	if(num==0){
		is_send_uart7=1;
		is_send_uart8=0;
	}else if(num==1){
		is_send_uart7=0;
		is_send_uart8=1;
	}else if(num==2){
		is_send_uart7=1;
		is_send_uart8=1;
	}
}

void vesc_set_rmp(int rpm)
{

	int32_t send_index = 0;
	uint8_t send_buffer[12]={0};
	send_buffer[send_index++] = COMM_SET_RPM;
	buffer_append_int32(send_buffer, rpm, &send_index);
	extern void(*send_func)(unsigned char *data, unsigned int len);
	send_func(send_buffer,5);
}

void vesc_smooth_set_vel(int rpm,int time_cnt)
{
	for(int i=0;i<time_cnt;++i){
		vesc_set_rmp(rpm*i/time_cnt);
		osDelay(1);
	}
}


