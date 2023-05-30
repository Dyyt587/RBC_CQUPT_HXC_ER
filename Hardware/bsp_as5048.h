#ifndef __BSP_AS5048_H
#define __BSP_AS5048_H

#include "main.h"


uint16_t SPI_AS5048A_1_ReadData(void);
uint16_t SPI_AS5048A_2_ReadData(void);
uint16_t SPI_AS5048A_3_ReadData(void);
uint16_t SPI_AS5048A_4_ReadData(void);
float GET_AS5048A_ANGLE(uint16_t data);
void Test_AS5048(void);
#endif

