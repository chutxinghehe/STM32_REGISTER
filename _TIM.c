#include "_TIM.h"

//______________________________________________________________________
//					
//----------------------------------------------------------------------
void init_interrupt_TIM4()
{
	RCC_enable_PERIPH(mAFIO);
	RCC_enable_PERIPH(mTIM4);
	
	TIM4->CR1 |= TIM_CR1_URS ; 	// only counter overflow generate an interrupt
	TIM4->CR1 &= ~TIM_CR1_DIR ;	// counter used as upcounter
	
	
	TIM4->CR1 |= TIM_CR1_CEN ;	// counter enable
	TIM4->DIER |= TIM_DIER_UIE;	// UIE: Update interrupt enable
	
	TIM4->PSC = (72 - 1) ;
	TIM4->ARR = 999 ;
	
	TIM4->EGR |= TIM_EGR_UG ;	// re-initialize
	
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 0);
	
	
}
/*
void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;
	if (myTick > 0)
    {
		myTick--;
    }
}
*/


//______________________________________________________________________
//					init TIM2
//----------------------------------------------------------------------
void init_TIM2()
{
	RCC_enable_PERIPH(mTIM2);
	TIM2->CR1 &= ~TIM_CR1_DIR ;
    TIM2->PSC = 72 - 1;        	// 72MHz / 72 = 1MHz (1 nhip = 1us)
    TIM2->ARR = 0xFFFF;        	// Ðem toi da 65535us
    TIM2->CR1 |= TIM_CR1_CEN;     	// Cho Timer chay tu do liên tuc
}


//______________________________________________________________________
//					delay
//----------------------------------------------------------------------
void delay_us(uint32_t us)
{
	TIM2->CNT = 0;
	while(TIM2->CNT < us);	// CNT là 16-bit, max = 65535
}



//______________________________________________________________________
//					delay
//----------------------------------------------------------------------
void initSystick()
{
	volatile static uint32_t tick = 0;
	
	SysTick_Config(SystemCoreClock / 1000);
}



void delay_ms(uint32_t ms);


