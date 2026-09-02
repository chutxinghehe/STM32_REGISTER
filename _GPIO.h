#ifndef	__GPIO_H
#define	__GPIO_H


//______________________________________________________________________
//					include
//----------------------------------------------------------------------
#include	<stdint.h>
#include <stm32f10x.h>


//______________________________________________________________________
//					define port name
//----------------------------------------------------------------------
#define PORTA		GPIOA
#define PORTB		GPIOB
#define PORTC		GPIOC


//______________________________________________________________________
//					pin mode
//----------------------------------------------------------------------
#define OUTPUT_MODE			((uint32_t) 0x01)
#define INPUT_MODE			((uint32_t) 0x02)


//______________________________________________________________________
//					INPUT MODES TYPE
//----------------------------------------------------------------------
#define INPUT_ANALOG               ((uint32_t)0x00)
#define INPUT_FLOATING             ((uint32_t)0x01)  //default at reset
#define INPUT_PU_PD                ((uint32_t)0x02)  //input with pull up or pull down


//______________________________________________________________________
//					OUTPUT MODES TYPE
//----------------------------------------------------------------------
#define OUTPUT_GEN_PURPOSE         ((uint32_t)0x00)  //general purpose output
#define OUTPUT_OD                  ((uint32_t)0x01)  //output open drain
#define OUTPUT_ALT_FUNCTION        ((uint32_t)0x02)  //Push Pull
#define OUTPUT_ALT_FUNCTION_OD     ((uint32_t)0x03)  //open drain


//______________________________________________________________________
//					PIN SPEED
//----------------------------------------------------------------------
#define SPEED_2MHZ			((uint32_t)0x02)
#define SPEED_10MHZ			((uint32_t)0x01)
#define SPEED_50MHZ			((uint32_t)0x03)



#define CNF_POS_1		(PINPOS[pin] + 2)
#define CNF_POS_2		(PINPOS[pin] + 3)


//______________________________________________________________________
//					Config Enum
//----------------------------------------------------------------------

typedef enum
{
	PULL_UP,
	PULL_DOWN
}pull_t;

//----------------------------------------------------------------------
//					Config Struct
//----------------------------------------------------------------------
typedef struct
{
	GPIO_TypeDef* port ;
	
	uint32_t pin ;
	
	uint32_t mode ;		// input or output
	
	uint32_t mode_type ;
	
	pull_t pull ;
	
	uint32_t speed ;
	
	uint32_t alt_fuc ;
	
}GPIO_Type ;



//______________________________________________________________________
//					Config Enum
//----------------------------------------------------------------------



typedef enum 
{
	RISING_EDGE,
	FALLING_EDGE,
	RISING_FALLING_EDGE
}edge_select;




//______________________________________________________________________
//						api init
//----------------------------------------------------------------------
void GPIO_Config_Pin(volatile GPIO_TypeDef* port, uint16_t pin, uint8_t mode);

void GPIO_Config_Pin_Speed(volatile GPIO_TypeDef* port, uint16_t pin, uint32_t speed, uint8_t mode);

//______________________________________________________________________
//						api config
//----------------------------------------------------------------------
void GPIO_Write(volatile GPIO_TypeDef* port, uint16_t pin, uint8_t state);
void GPIO_Toggle(volatile GPIO_TypeDef* port, uint16_t pin);
uint8_t GPIO_Read(volatile GPIO_TypeDef* port, uint16_t pin);

void gpio_init(GPIO_Type cfg);


//______________________________________________________________________
//						INTERRUPT
//----------------------------------------------------------------------
void config_gpio_interrupt(GPIO_TypeDef* port, uint32_t pin, edge_select edge);
void enable_gpio_interrupt(uint32_t pin, IRQn_Type irqNumber);
void clear_interrupt(uint32_t pin);





#endif