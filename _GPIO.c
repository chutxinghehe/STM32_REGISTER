#include "_GPIO.h"

/*---Every pin in the high and low control registers has 4 associated bits
thus the position of every pin is shifted 4 bits this array keeps track
of how much each pinNumber has to be shifted to be in the proper bit
*/
uint32_t PINPOS[16] = { //takes me to the 1st BIT in MODE
    (0x00), //pin0 position starts at 0
    (0x04), //pin1 position starts at 4
    (0x08),
    (0x0C),
    (0x10),
    (0x14),
    (0x18),
    (0x1C),
    (0x00), // pin 8
    (0x04), // pin 9
    (0x08),
    (0x0C),
    (0x10),
    (0x14),
    (0x18),
    (0x1C)
};

//______________________________________________________________________
//					congif pin
//----------------------------------------------------------------------
void GPIO_Config_Pin(volatile GPIO_TypeDef* port, uint16_t pin, uint8_t mode)
{
	// 
	volatile uint32_t* CRx = (pin >= 8) ? &port->CRH : &port->CRL;

	// vi tri bit CNF0 cua pin trong thanh ghi tuong ung
	uint32_t cnf_pos = PINPOS[pin] + 2U;


	*CRx &= ~(0x3U << cnf_pos);

	switch (mode)
	{
		case OUTPUT_GEN_PURPOSE | INPUT_ANALOG:    //  -- CNF = 00
				// bits da xoa o tren, khong can ghi them
		break;

		case 0x01:  // OUTPUT_OD | INPUT_FLOATING             -- CNF = 01
			*CRx |= (0x1U << cnf_pos);
		break;

		case 0x02:  // OUTPUT_ALT_FUNCTION | INPUT_PU_PD      -- CNF = 10
			*CRx |= (0x2U << cnf_pos);
		break;

		case 0x03:  // OUTPUT_ALT_FUNCTION_OD                 -- CNF = 11
			*CRx |= (0x3U << cnf_pos);
		break;
}//end switch

}

//______________________________________________________________________
//					PIN SPEED
//----------------------------------------------------------------------
void GPIO_Config_Pin_Speed(volatile GPIO_TypeDef* port, uint16_t pin, uint32_t speed, uint8_t mode)
{
	volatile uint32_t* CRx = (pin >= 8) ? &port->CRH : &port->CRL;

	// xoa 2 bit MODE  ( AKA input mode )
	*CRx &= ~(0x3U << PINPOS[pin]);

	if (mode != INPUT_MODE)
	{
		*CRx |= (speed << PINPOS[pin]);
	}
}

//______________________________________________________________________
//
//----------------------------------------------------------------------
void GPIO_Write(volatile GPIO_TypeDef* port, uint16_t pin, uint8_t state)
{
	if (state)
		port->BSRR = (1U << pin);
	else
		port->BRR  = (1U << pin);
}

//______________________________________________________________________
//
//----------------------------------------------------------------------
void GPIO_Toggle(volatile GPIO_TypeDef* port, uint16_t pin)
{
	port->ODR ^= (1U << pin);
}

//______________________________________________________________________
//
//----------------------------------------------------------------------
uint8_t GPIO_Read(volatile GPIO_TypeDef* port, uint16_t pin)
{
	return (uint8_t)((port->IDR >> pin) & 0x01);
}

//______________________________________________________________________
//
//----------------------------------------------------------------------
void gpio_init(GPIO_Type cfg)
{
    // enable clock
    if (cfg.port == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    if (cfg.port == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    if (cfg.port == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	
    GPIO_Config_Pin_Speed(cfg.port, cfg.pin, cfg.speed, cfg.mode);
    GPIO_Config_Pin(cfg.port, cfg.pin, cfg.mode_type);

	
	if (  (cfg.mode == INPUT_MODE) && (cfg.mode_type == INPUT_PU_PD)  )
	{
		if (cfg.pull == PULL_UP)
		{
			cfg.port->BSRR = (1U << cfg.pin);   // pull-up
		}
		
		else if (cfg.pull == PULL_DOWN)
		{
			// don't need to use |= cause this reg is write only
			cfg.port->BRR = (1U << cfg.pin);	// pull down 
		}
	}

}

//______________________________________________________________________
//					INTERRUPT
//----------------------------------------------------------------------
void config_gpio_interrupt(GPIO_TypeDef* port, uint32_t pin, edge_select edge)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	//------------------------------------------------
	uint32_t afio_exti = 0;
	uint32_t shift = (pin % 4) * 4 ;
	
	
	if (port == GPIOA)			afio_exti = 0x00;
	else if (port == GPIOB)		afio_exti = 0x01;
	else if (port == GPIOC)		afio_exti = 0x02;
	
	
	if (pin < 4)
	{
		AFIO->EXTICR[0] &= ~(0x0F << shift) ;
		AFIO->EXTICR[0] |= (afio_exti << shift) ;
	}
	else if (pin < 8)
	{
		AFIO->EXTICR[1] &= ~(0x0F << shift) ;
		AFIO->EXTICR[1] |= (afio_exti << shift) ;
	}
	else if (pin < 12)
	{
		AFIO->EXTICR[2] &= ~(0x0F << shift) ;
		AFIO->EXTICR[2] |= (afio_exti << shift) ;
	}
	else if (pin < 16)
	{
		AFIO->EXTICR[3] &= ~(0x0F << shift) ;
		AFIO->EXTICR[3] |= (afio_exti << shift) ;
	}
	
	
	//------------------------------------------------
	if (edge == RISING_EDGE)
	{
		EXTI->RTSR |= (1 << pin);
	}
	else if (edge == FALLING_EDGE)
	{
		EXTI->FTSR |= (1 << pin);
	}
	else if (edge == RISING_FALLING_EDGE)
	{
		EXTI->RTSR |= (1 << pin);
		EXTI->FTSR |= (1 << pin);
	}
}

//______________________________________________________________________
//					INTERRUPT enable
//----------------------------------------------------------------------
void enable_gpio_interrupt(uint32_t pin, IRQn_Type irqNumber)
{
	// enable interrup mask
	EXTI->IMR |= (1 << pin) ;

	//enable interrupt in nvic
	NVIC_EnableIRQ(irqNumber);
}

//______________________________________________________________________
//
//----------------------------------------------------------------------
void clear_interrupt(uint32_t pin)
{
	EXTI->PR = (1 << pin);
}










//______________________________________________________________________
//______________________________________________________________________
//______________________________________________________________________
//______________________________________________________________________
//	GPIO_Type button;
//	button.port	= GPIOB;
//	button.pin	= 0;
//	button.mode	= INPUT_MODE;
//	button.mode_type = INPUT_PU_PD;
//	button.pull = PULL_DOWN;
//	gpio_init(button);



//-----------------------------------






