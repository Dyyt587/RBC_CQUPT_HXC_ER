#include "led_task.h"
#include "cmsis_os.h"
#include "test_task.h"
#include "bsp_as5048.h"
#include "vofa_lower.h"
#include "bldc_interface.h"
#include "bsp_sbus.h"
#include "bsp_vesc.h"
#include "can.h"
#include "bsp_can.h"
#include "stdio.h"
#include "pid.h"
#include "vofa_lower.h"
#include "shell.h"
#include "commend.h"
#include <stdarg.h>
#include <stdlib.h>
#include "log.h"


void test_task(void const * argument)
{
	const rc_info_t* _rc = get_remote_control_point();
	Init_test_AS5048();
	while(1)
	{

		Test_AS5048();


		osDelay(5);

	}
}






