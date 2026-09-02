#ifndef _SYS
#define _SYS




#include "stm32f10x.h"



void systick_init();
void SysTick_Handler();
void delay_ms(uint32_t ms);



#endif