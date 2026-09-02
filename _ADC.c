#include "_ADC.h"
#include "_GPIO.h"
#include "_TIM.h"


//____________________________________________________________________________________________________________________________
//					ADC TO READ POTENTIAL (USE FOR 72MHZ)
//----------------------------------------------------------------------------------------------------------------------------
/*
	------------------------------------------------------------
	- this functinois use to read the potential
	- gpio use in this case is Ch5 which mean use the pin A5
	- that use the continues mode and don't use any interrupt
	
	------------------------------------------------------------
	(if u want to use interrupt :)
			ADC2->CR1 |= ADC_CR1_EOCIE ;	// interrupr generated
			NVIC_EnableIRQ(ADC1_2_IRQn) ;	// en nvic interrupt
	
	(and call the function handler:)
			void ADC1_2_IRQHandler()
			{
				while (!(ADC2->SR & ADC_SR_EOC));
	
				check = (ADC2->DR & 0x0FFF) ;
				// interrupt flag auto clear when i read SR register
			}
	------------------------------------------------------------
*/
void ADC2_CH5_Init()
{
	RCC_enable_PERIPH(mADC2);
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8 ;		// 72Mhz / 8 = 9Mhz for ADC
	
	GPIO_Type pot;
	pot.port	= GPIOA;
	pot.pin		= 5;
	pot.mode	= INPUT_MODE;
	pot.mode_type 	= INPUT_ANALOG;
	gpio_init(pot);
	
	
	
	//----------------------------------------------------

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
}


//-------------------------------------------------------------------------------
// this function read the data from 0 to 4095
uint16_t value_ADC2_CH5()
{
	// (End of conversion)
    while (!(ADC2->SR & ADC_SR_EOC));
	
	return (uint16_t)(ADC2->DR & 0x0FFF) ;
}




//____________________________________________________________________________________________________________________________
//				
//----------------------------------------------------------------------------------------------------------------------------




































