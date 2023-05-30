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
#ifndef _VOFA_LOWE_FIREWATER_H
#define _VOFA_LOWE_FIREWATER_H

#ifdef __cplusplus
extern "C"
{
#endif
//////////////////////////////////////////////////////////////////////////////
    /// 支持协议说明：
    /// firewater
    ///     文本以\n \r\n \n\r 结尾
    /// justfloat
    ///     文本以{ 0x00, 0x00, 0x80, 0x7f }结尾
    /// rawdata
    ///     直接输出，不做任何处理
    ///
    /// 本文件已经实现以上协议的特殊处理
    /// 未使用异步收发模式，可由用户自行实现
    ///
////////////////////////////////////////////////////////////////////////////
#define VOFA_LINE_FEED "\n" //换行符
#define VOFA_RX_BUF_SIZE 256 
typedef  char vofa_char_t;
typedef unsigned short vofa_size_t;



	typedef enum {
		Format_Invalid = 0U,
		Format_Mono,
		Format_MonoLSB,
		Format_Indexed8,
		Format_RGB32,
		Format_ARGB32,
		Format_ARGB32_Premultiplied,
		Format_RGB16,
		Format_ARGB8565_Premultiplied,
		Format_RGB666,
		Format_ARGB6666_Premultiplied,
		Format_RGB555,
		Format_ARGB8555_Premultiplied,
		Format_RGB888,
		Format_RGB444,
		Format_ARGB4444_Premultiplied,
		Format_RGBX8888,
		Format_RGBA8888,
		Format_RGBA8888_Premultiplied,
		Format_BGR30,
		Format_A2BGR30_Premultiplied,
		Format_RGB30,
		Format_A2RGB30_Premultiplied,
		Format_Alpha8,
		Format_Grayscale8,
        // 以下格式发送时，IMG_WIDTH和IMG_HEIGHT不需要强制指定，设置为-1即可
		Format_BMP,
		Format_GIF,
		Format_JPG,
		Format_PNG,
		Format_PBM,
		Format_PGM,
		Format_PPM,
		Format_XBM,
		Format_XPM,
		Format_SVG,
	}vofa_img_format_t;

	typedef enum {
		VOFA_NONE = 0U,
		VOFA_ERROR_INVAI_POTOCOL, //使用错误的协议或函数不支持该协议
		VOFA_ERROR_DATA, //输入参数错误
		VOFA_ERROR_BOTTOM, //底层错误

		VOFA_ERROE_NUKNOWN,

	}vofa_status_t;

	typedef enum {
		VOFA_FIREWATER = 0U,
		VOFA_JUSTFLOAT,
		VOFA_RAWDATA,
	}vofa_protocol_t;

	typedef struct {
		vofa_size_t id;
		vofa_size_t size;
		vofa_size_t height;
		vofa_size_t width;
		vofa_img_format_t format;
	}vofa_img_t;

	typedef struct vofa_var_t {
		float* variable;
		vofa_char_t* name;
		struct vofa_var_t* next;
	}vofa_variable_t;

	////////////////////////////以下函数应由用户调用//////////////////////////////////////////////
	vofa_status_t vofa_sends(vofa_char_t* pdata, vofa_size_t length);
	vofa_status_t vofa_send(vofa_char_t data);
	vofa_status_t vofa_send_protocol(vofa_char_t* name, float pdata[], vofa_size_t length);
	vofa_status_t vofa_send_img(vofa_size_t id, vofa_size_t size, vofa_size_t height, vofa_size_t width, vofa_img_format_t format, vofa_char_t* pdata, vofa_size_t length);
	vofa_status_t vofa_send_imgf(vofa_img_t* format, vofa_char_t* pdata, vofa_size_t length);
	vofa_status_t vofa_register_variable(float* variable, vofa_char_t* name);
	vofa_status_t vofa_delete_variable(float* variable);
	vofa_status_t vofa_change_protocol(vofa_protocol_t protocol);
	int           vofa_printf(const char* format, ...);

	/////////////////////////////////THE END////////////////////////////////////////////////////

	////////////////////////////以下函数应由用户移植或实现///////////////////////////////////////

	vofa_status_t vofa_output(vofa_char_t* pdata, vofa_size_t length);

	vofa_status_t vofa_input_handler(vofa_char_t data); //该函数未实现!!!!!

	void vofa_init(void);
	/////////////////////////////////THE END//////////////////////////////////////////////////





#ifdef __cplusplus
}
#endif
#endif


