/*************************************************
Copyright .
File name: vofa_lower_firewater.h
Author: 杨应添(dyyt)
Version: 1.0 2022.01.19
Description: 实现vofa的通讯协议
Others: 经过实验，vofa_send_protocol的name没啥用(可能给人看)，还占带宽
        图片未验证！！！！！！！
        基于tc264和逐飞V3库，依赖仅uart_write_buffer();及相关初始化函数
        若基于逐飞V3库，且使用uart0,无需改动本文件即可实现发送功能
        上位机发送到下位机的解析函数未实现！！！！！

Log:

    V1.0 dyyt完善文档及测试历程
    V0.4 dyyt修改vofa_send和vofa_sends，适配justfloat引擎，可开启数据过滤
    V0.3 dyyt修复vofa_send_protocol在justfloat引擎下数据显示错误ps:just发送float
    V0.2 dyyt修复vofa_send_protocol最后一个数据显示错误ps:数组越界
    V0.1 dyyt初次创建
*************************************************/
#pragma diag_suppress 188


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "vofa_lower.h"
	  #include "stm32f4xx_hal.h"
    #include "usart.h"
#ifndef nullptr
#define nullptr (0U)
#endif
static vofa_variable_t *start_variable;
vofa_protocol_t _protocol = VOFA_FIREWATER;
//vofa_protocol_t _protocol = VOFA_JUSTFLOAT;
static unsigned char tail[4]={ 0x00, 0x00, 0x80, 0x7f };

/**
 * @brief.类似printf
 * @code .vofa_printf("%s:%f,%f\n", name, data0, data1);
 */
int vofa_printf(const char* format, ...)
{
	va_list va;
    unsigned char tail[4] = {0x00,0x00,0x80,0x7f};
	va_start(va, format);
	char buffer[256];
	int rec = vsnprintf((char*)buffer, sizeof(buffer)+1, format, va);
	va_end(va);
    vofa_output((vofa_char_t*)&buffer, (vofa_size_t)rec);
	switch (_protocol)
	{
	case VOFA_FIREWATER:
		break;
	case VOFA_JUSTFLOAT:
		vofa_output((vofa_char_t*)tail, 4);
		break;
	case VOFA_RAWDATA:
		break;
	default:
		break;
	}
	return rec;
}
/**
 * @brief .用于多个数据的直接输出，用firewater引擎的话要自己加\n，justfloat不用加帧尾
 * @param .传输数据的地址
 * @param .数据长度
 * @return .状态
 * @see 它是直接输出的
 *
*/
vofa_status_t vofa_sends(vofa_char_t* pdata, vofa_size_t length)
{
    vofa_status_t rec = VOFA_ERROE_NUKNOWN;
    rec = vofa_output(pdata, length);
    switch (_protocol)
    {
    case VOFA_FIREWATER:
        //rec = vofa_output((vofa_char_t*)&("\n"), (vofa_size_t)sizeof(str));
        break;
    case VOFA_JUSTFLOAT:
        rec = vofa_output((vofa_char_t*)tail, (vofa_size_t)sizeof(tail));
        break;
    case VOFA_RAWDATA:
        return VOFA_ERROR_INVAI_POTOCOL;

    default:
        return VOFA_ERROR_INVAI_POTOCOL;
    }
    return rec;
}
/**
* @brief .用于单个数据（一个字节）的直接输出
* @param .传输的数据
* @return .状态
* @see 它是直接输出的
*
*/
vofa_status_t vofa_send(vofa_char_t data)
{
    vofa_status_t rec = VOFA_ERROE_NUKNOWN;
	rec = vofa_output(&data, 1);
	switch (_protocol)
	    {
	    case VOFA_FIREWATER:
	        //rec = vofa_output((vofa_char_t*)&("\n"), (vofa_size_t)sizeof(str));
	        break;
	    case VOFA_JUSTFLOAT:
	        rec = vofa_output( (vofa_char_t*)tail, (vofa_size_t)sizeof(tail));
				  break;
	    case VOFA_RAWDATA:
	        return VOFA_ERROR_INVAI_POTOCOL;
	    default:
	        return VOFA_ERROR_INVAI_POTOCOL;
	    }
	    return rec;
}
/**
 * @brief .用于多个数据的直接输出
 * @param .传输变量的名字，不用时传入0即可
 * @param .传输变量数量,注意
 * @param .数据,可以有多个,不支持通配符
 * @return .状态
 * @see 该函数可为阻塞制，也可不阻塞，由宏配置
 *	eg: vofa_send_protocol(&(curve), buffer, 1);
 *	eg: vofa_send_protocol((vofa_char_t*)0, buffer, 3);
*/
vofa_status_t vofa_send_protocol(vofa_char_t* name, float pdata[], vofa_size_t length)
{
	switch (_protocol)
	{
	case VOFA_FIREWATER:
		if (name)vofa_printf("%s:", name);
		for(vofa_size_t i = 0; (i < length-1); ++i)
		{
			vofa_printf("%f,", pdata[i]);
		}
        vofa_printf("%f", pdata[length-1]);
		vofa_printf(VOFA_LINE_FEED);
		break;
	case VOFA_JUSTFLOAT:
/*		for(vofa_size_t i = 0; i < length-1;++i)
		{
			vofa_printf("%f", pdata[i]);
		}*/
	    vofa_output((vofa_char_t*)pdata, length*4);
		vofa_output((vofa_char_t*)tail, 4);
		break;
	case VOFA_RAWDATA:
		return VOFA_ERROR_INVAI_POTOCOL;

	default:
		return VOFA_ERROR_INVAI_POTOCOL;
	}
    return VOFA_NONE;

}
/**
 * @brief .用于传输照片
 * @param .传输照片的id，此ID用于标识不同图片通道,最大支持范围见宏VOFA_MAX_IMG_ID
 * @param .传输照片的数据大小
 * @param .传输照片的数据宽度
 * @param .传输照片的数据高度
 * @param .传输照片格式
 * @param .传输照片的数据地址
 * @param .数据长度
 * @return .状态
 * @see 该函数可为阻塞制，也可不阻塞，由宏配置
 *
*/
vofa_status_t vofa_send_img(vofa_size_t id, vofa_size_t size, vofa_size_t height,
	vofa_size_t width, vofa_img_format_t format, vofa_char_t* pdata, vofa_size_t length)
{
	 vofa_img_t img={0};
     img.id=id;
     img.size=size;
     img.height=height;
     img.width=width;
     img.format=format;
	return vofa_send_imgf(&img, pdata, length);
}
/**
 * @brief .用于传输照片,传入参数与vofa_send_img不同
 * @param .传输照片的数据配置项
 * @param .传输照片的数据地址
 * @param .数据长度
 * @return .状态
 * @see 该函数可为阻塞制，也可不阻塞，由宏配置
 *
*/
vofa_status_t vofa_send_imgf(vofa_img_t* format, vofa_char_t* pdata, vofa_size_t length)
{
    int preFrame[7] = {
        (int)format->id,
        (int)format->size,
        (int)format->width,
        (int)format->height,
        (int)format->format,
        0x7F800000,
        0x7F800000
    };
	vofa_status_t rec = VOFA_ERROE_NUKNOWN;
	switch (_protocol)
	{
	case VOFA_FIREWATER:
		rec = vofa_printf("image:%d,%d,%d,%d,%d\n", format->id, format->size, format->width, format->height, format->format);
		break;
	case VOFA_JUSTFLOAT:
		// 先发送前导帧

		rec = vofa_output((vofa_char_t*)preFrame, (vofa_size_t)sizeof(preFrame));
		break;
	case VOFA_RAWDATA:
		return VOFA_ERROR_INVAI_POTOCOL;

	default:
		return VOFA_ERROR_INVAI_POTOCOL;
	}

	rec = vofa_output(pdata, length);
	return rec;
}
/**
 * @brief .用于将变量注册到一个通道，使得上位机能更改该变量
 * @param .变量地址
 * @param .变量名字
 * @param .数据长度
 * @return .状态
 * @see
 *
*/
vofa_status_t vofa_register_variable(float* variable, vofa_char_t* name)
{
	if (start_variable == nullptr)//第一次注册时特殊化处理
	{
		start_variable = (vofa_variable_t*)malloc(sizeof(vofa_variable_t));
		start_variable->name = name;
		start_variable->variable = variable;
		return VOFA_NONE;
	}

	vofa_variable_t* p = start_variable;
	while (p->next)
	{
		p = p->next;
	}//找到变量表的最后面一个变量
	p->next = (vofa_variable_t*)malloc(sizeof(vofa_variable_t));
	p->next->name = name;
	p->next->variable = variable;
	p->next->next = nullptr;
	return VOFA_NONE;
}
/**
 * @brief .用于将注册变量删除
 * @param .变量地址
 * @see
 *
*/
vofa_status_t vofa_delete_variable(float* variable)
{
	if (start_variable->variable == variable)
	{
		start_variable = start_variable->next;
		return VOFA_NONE;
	}
	vofa_variable_t* p_0 = start_variable;//要删除的变量的前一个变量
	vofa_variable_t* p = start_variable->next; //要删除的变量
	while (p)//还没到头
	{
		if (p->variable == variable)
		{
			p_0->next = p->next;
			return VOFA_NONE;
		}
		p_0 = p;
		p = p->next;
	}
	return VOFA_ERROR_DATA;
}
/**
 * @brief .改变vofa传输协议
 * @param .协议
 * @return .状态
 * @see 支持firewater，justfloat，rawdata
 *
*/
vofa_status_t vofa_change_protocol(vofa_protocol_t protocol)
{
	_protocol = protocol;
	return VOFA_NONE;
}

/**
 * @brief .用于vofa输入命令和解析使用，应该在每次接收到数据(一个字节)时调用(建议中断中调用)
 * @param .一个字节的输入数据
 * @return .none
 * @see 放在中断中运行，该函数还未实现(●'◡'●)
 *
*/
vofa_status_t vofa_input_handler(vofa_char_t data)
{
	//	static vofa_char_t rx_buf[VOFA_RX_BUF_SIZE] = { 0 };
	//	static int index_rx = 0;
	//	//前导变量名最大31个字符
	//	//static char str[32];
	//	static float data;
	//	static vofa_variable_t* this_var;
	//	if (data == ":") //英文键盘
	//	{
	//		vofa_variable_t* p = start_variable;
	//
	//		while (p)//还没到头
	//		{
	//			for (int i = 0; i < index_rx; ++i){
	//
	//				if (p->name[i] != rx_buf[i]) goto THIS;
	//				//找到变量了
	//				this_var = p;
	//				goto AHEAD;
	//			}
	//THIS:		p = p->next;
	//		}
	//		return VOFA_ERROR_DATA;
	//	}
	//AHEAD: 
	//	rx_buf[index_rx++] = data; //存入buf
    return VOFA_NONE;

}
/////////////////////////////////THE END////////////////////////////////////////////////////

////////////////////////////以下函数应由用户移植或实现///////////////////////////////////////
/**
 * @brief .用于vofa的输出，由用户实现
 * @param .传输数据的地址
 * @param .数据长度
 * @return .none
 * @see 该函数为阻塞制，返回即表明发送已完成
 *
*/
vofa_status_t vofa_output(vofa_char_t* pdata, vofa_size_t length)
{
	
    HAL_UART_Transmit(&huart6,(uint8_t*)pdata,length,0xffff);
    //HAL_UART_Transmit_DMA(&huart6,(uint8_t*)pdata,length);
    return VOFA_NONE;
}

/**
 * @brief .初始化vofa，一般进行串口初始化，变量注册
 *
*/
void vofa_init(void)
{
    start_variable=nullptr;//不要删，保险点
	//初始化输出
	//uart_init(uart_n, baud, tx_pin, rx_pin);//默认使用uart0，故不用再次初始化

	//将变量注册

}
