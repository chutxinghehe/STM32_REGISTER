#include "_ADC.h"
#include "_GPIO.h"
#include "_TIM.h"


//____________________________________________________________________________________________________________________________
//					ADC TO READ POTENTIAL (USE FOR 72MHZ)
//----------------------------------------------------------------------------------------------------------------------------
/** 1 channel adc -> PA5
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







//____________________________________________________________________________________________________________________________
//					ADC TO READ joystick (USE FOR 72MHZ)
//----------------------------------------------------------------------------------------------------------------------------
/** @idk  use 2 channel; PA5 -> VRx          PA7 ->VRy       && use DMA1

	- don't use interrup
	- use adc1 (channel 5 and 7); dma1
	- use dma; circular buffer; continous; ....
	
	@note DMA will auto clear 'EOC bit'   so -> ignore to clear this bit
	@note Pin '5v' of joystick need to wire to pin '3v3' in stm32
*/
static volatile uint16_t mSample[2] = {0, 0};


void Init_readJoystick()
{
	//---------------------------------------
	RCC_enable_PERIPH(mADC1);
	RCC->AHBENR |= RCC_AHBENR_DMA1EN ;
	
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8 ;		// 72Mhz / 8 = 9Mhz for ADC
	RCC->AHBENR |= RCC_AHBENR_DMA1EN ;			// DMA1
	
	
	GPIO_Type joy_x;
	joy_x.port	= GPIOA;
	joy_x.pin		= 5;
	joy_x.mode	= INPUT_MODE;
	joy_x.mode_type 	= INPUT_ANALOG;
	gpio_init(joy_x);
	
	
	GPIO_Type joy_y;
	joy_y.port	= GPIOA;
	joy_y.pin	= 7;
	joy_y.mode	= INPUT_MODE;
	joy_y.mode_type 	= INPUT_ANALOG;
	gpio_init(joy_y);
	
	//_____________________________ CONFIG ADC AND DMA __________________________________________
	
	// channel and sample time
	ADC1->SMPR2 |= ADC_SMPR2_SMP5_2 | ADC_SMPR2_SMP5_1 | ADC_SMPR2_SMP5_0 ;	
	ADC1->SMPR2 |= ADC_SMPR2_SMP7_2 | ADC_SMPR2_SMP7_1 | ADC_SMPR2_SMP7_0 ;	
	
	
	// clear bit in length to pick 2 conversion
	ADC1->SQR1 &= ~(ADC_SQR1_L) ;
	ADC1->SQR1 |= (ADC_SQR1_L_0) ;
	
	
	// init the firt conversion &  2rd
	ADC1->SQR3 |= (ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2) ;						// write channel 5 in 1st conversion
	ADC1->SQR3 |= (ADC_SQR3_SQ2_0 | ADC_SQR3_SQ2_1 | ADC_SQR3_SQ2_2) ;		// WRITE_REG channel 7 in 2st conversion
	
	
	// scan mode and enable dma
	ADC1->CR1 |= ADC_CR1_SCAN ;
	ADC1->CR2 |= ADC_CR2_DMA ;
	
	
	// CONFIG DMA
	DMA1_Channel1->CPAR = ((uint32_t) (&(ADC1->DR)) );		// address of periph
	DMA1_Channel1->CMAR = (uint32_t) mSample ;			// addr of memory
	DMA1_Channel1->CNDTR = 2; 				//number of the data transfer
	DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0 	;	
	DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0 ;
	DMA1_Channel1->CCR |= DMA_CCR1_MINC | DMA_CCR1_CIRC | DMA_CCR1_EN ;		// Memory increase mode ; circular ; enable
	
	
	ADC1->CR2 |= ADC_CR2_CONT ;			// continous conversion
	ADC1->CR2 |= ADC_CR2_ADON ;			// wake up the adc
	
	delay_us(1);
	
	
	ADC1->CR2 |= ADC_CR2_RSTCAL;	// reset calib
    while (ADC1->CR2 & ADC_CR2_RSTCAL);
	
	ADC1->CR2 |= ADC_CR2_CAL ;		//calibr  
	while (ADC1->CR2 & ADC_CR2_CAL);
	
	ADC1->CR2 |= ADC_CR2_ADON ;			// again -> start
	delay_us(1);
}


uint16_t readVRX()
{
	return mSample[0]; // Channel 5 (Rank 1)
}

uint16_t readVRY()
{
	return mSample[1]; // Channel 7 (Rank 2)
}

//



















