#ifndef __ft800_H
#define __ft800_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "ft_gpu.h"

#define FT800_PD_N	GPIO_PIN_0
#define FT800_CS_N	GPIO_PIN_1
#define LCD_WQVGA

void ft800cmdWrite(unsigned char);
void ft800memWrite8(unsigned long, unsigned char);
void ft800memWrite16(unsigned long, unsigned int);
void ft800memWrite32(unsigned long, unsigned long);
unsigned char ft800memRead8(unsigned long);
unsigned int ft800memRead16(unsigned long);
unsigned long ft800memRead32(unsigned long);
unsigned int incCMDOffset(unsigned int, unsigned char);
void ft800_Init(void);

#endif /*__ft800_H */
