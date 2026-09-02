#include "_RCC.h"


//______________________________________________________________________
//					
//----------------------------------------------------------------------
void RCC_enable_PortA()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
}


//______________________________________________________________________
//					
//----------------------------------------------------------------------
void RCC_enable_PortB()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN ;
}


//______________________________________________________________________
//					
//----------------------------------------------------------------------
void RCC_enable_PortC()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN ;
}


//______________________________________________________________________
//					
//----------------------------------------------------------------------
void RCC_enable_AFIO()
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN ;
}


//______________________________________________________________________
//					
//----------------------------------------------------------------------
void RCC_enable_PERIPH(myPeriph_t per)
{
	switch (per)
    {
        case mTIM2: 	
			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
		break;
		
		
        case mTIM3: 	
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
		break;
		
		
        case mTIM4: 	
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; 
		break;
		
		
        case mAFIO:		
			RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 
		break;
      
		
		case mADC1:		
			RCC->APB2ENR |= RCC_APB2ENR_ADC1EN ;
		break;
		
		
		case mADC2:		
			RCC->APB2ENR |= RCC_APB2ENR_ADC2EN ;
		break;
		
        default: break;
    }
}


//______________________________________________________________________
//					
//----------------------------------------------------------------------








