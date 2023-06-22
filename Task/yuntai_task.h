#ifndef __TEST_TASK_H
#define __TEST_TASK_H

#include "main.h"
#include "struct_typedef.h"
#include "struct_typedef.h"
#include "pid.h"
#include "bsp_sbus.h"
/**
  * @brief          led rgb task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          led RGBÈÎÎñ
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern void yuntaiTask(void const * argument);
void motors_init(void);
void pid_shoe(pid_t*pid);
void motor_handler(const rc_info_t* _rc);
void steer_and_5065motor_handler(const rc_info_t* _rc);
#endif


