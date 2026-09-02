#include <stm32f10x.h>
#include "_systick.h"
#include "_GPIO.h"
#include "_RCC.h"	
#include "_TIM.h"	
#include "_ADC.h"


volatile uint32_t check;




int main(void)
{
	initSystick();
	
	init_TIM2();
	
	
	//---------------------------------------
	GPIO_Type PC13;
	PC13.port	= GPIOC;
	PC13.pin	= 13;
	PC13.mode	= OUTPUT_MODE;
	PC13.mode_type	= OUTPUT_GEN_PURPOSE;
	PC13.speed		= SPEED_50MHZ;
	gpio_init(PC13);
	
	


	
	//---------------------------------------
	RCC_enable_PERIPH(mADC2);
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8 ;		// 72Mhz / 8 = 9Mhz for ADC
	
	GPIO_Type pot;
	pot.port	= GPIOA;
	pot.pin		= 5;
	pot.mode	= INPUT_MODE;
	pot.mode_type 	= INPUT_ANALOG;
	gpio_init(pot);
	
	
	
	ADC2->CR1 |= ADC_CR1_EOCIE ;	// interrupr generated
	NVIC_EnableIRQ(ADC1_2_IRQn) ;	// en nvic interrupt
	
	ADC2->SMPR2 |= ADC_SMPR2_SMP5_2 | ADC_SMPR2_SMP5_1 | ADC_SMPR2_SMP5_0 ;		// channel and sample time
	ADC2->SQR1 &= ~(ADC_SQR1_L) ;		// clear bit in length to pick 1 conversion
	ADC2->SQR3 |= (ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2) ;		// init the firt conversion
	
	ADC2->CR2 |= ADC_CR2_CONT ;			// continous conversion
	ADC2->CR2 |= ADC_CR2_ADON ;			// wake up the adc
	
	delay_us(1);
	
	ADC2->CR2 |= ADC_CR2_RSTCAL;	// reset calib
    while (ADC2->CR2 & ADC_CR2_RSTCAL);
	
	ADC2->CR2 |= ADC_CR2_CAL ;		//calibr  
	while (ADC2->CR2 & ADC_CR2_CAL);
	
	ADC2->CR2 |= ADC_CR2_ADON ;			// again -> start
	delay_us(1);
	
	
	
	
	
	while (1)
	{
     
		if (check > 2000)
		{
			GPIO_Write(PORTC, 13, 1);
			delay_ms(1000);
			GPIO_Write(PORTC, 13, 0);
			delay_ms(1000);
		}
		else
		{
			GPIO_Write(PORTC, 13, 0);
			delay_ms(10);
			GPIO_Write(PORTC, 13, 1);
			delay_ms(100);
		}
	}


	
}















	//---------------------------------------
//	GPIO_Type button;
//	button.port	= GPIOB;
//	button.pin	= 0;
//	button.mode	= INPUT_MODE;
//	button.mode_type = INPUT_PU_PD;
//	button.pull = PULL_DOWN;
//	gpio_init(button);

