#include <stm32f10x.h>
#include "_systick.h"
#include "_GPIO.h"
#include "_RCC.h"	
#include "_TIM.h"	
#include "_ADC.h"


int main(void)
{
	initSystick();
	init_TIM2();
	
	
	
	//_______________	INIT GPIO	___________________________________________________________
	GPIO_Type PC13;
	PC13.port	= GPIOC;
	PC13.pin	= 13;
	PC13.mode	= OUTPUT_MODE;
	PC13.mode_type	= OUTPUT_GEN_PURPOSE;
	PC13.speed		= SPEED_50MHZ;
	gpio_init(PC13);
	
	
	
	
	
	
	while (1)
	{
		
		
		

	}


	
}








