#ifndef __BSP_VESC
#define __BSP_VESC

#ifdef STM32F4
#include "stm32f4xx_hal.h"
#elif defined STM32F1
#include "stm32f1xx_hal.h"
#endif

#include "stm32f4xx.h"

void vesc_init(void);
void vesc_set_point(uint8_t num);
void vesc_set_rmp(int rpm);





#endif

