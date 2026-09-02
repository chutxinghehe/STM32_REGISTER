#include "_systick.h"


static volatile uint32_t myTick = 0 ;


void systick_init()
{
	SystemCoreClockUpdate();
	
	SysTick_Config(SystemCoreClock / 1000);
}


void SysTick_Handler()
{
	if (myTick > 0)		
	{	
		myTick-- ;	
	}
}


void delay_ms(uint32_t ms)
{
	myTick = ms;
	while (myTick > 0);
}






