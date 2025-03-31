/**
 * @file LLD_GPIO.h
 * @author ACR
 * @brief Header file for GPIO peripheral
 * @details
**/

#ifndef LLD_GPIO_H_
#define LLD_GPIO_H_

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <avr/io.h>
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
typedef enum
{
	E_LEFT_TRIGGER,
	E_CENTER_TRIGGER,
	E_RIGHT_TRIGGER,

	E_LEFT_ECHO,
	E_CENTER_ECHO,
	E_RIGHT_ECHO,

	E_LED_GREEN,
	E_LED_ORANGE,
	E_LED_RED,
	E_LED_YELLOW_1,
	E_LED_YELLOW_2,
	E_LED_YELLOW_3
	
}GPIO;

/*--------------------------------------------------------------------------*/
/*! ... GLOBAL FUNCTIONS DECLARATIONS ...                                   */
/*--------------------------------------------------------------------------*/
void LLD_GPIO_Init(void);
void LLD_GPIO_WritePin(GPIO e_Gpio);
void LLD_GPIO_ClearPin(GPIO e_Gpio);
uint8_t LLD_GPIO_ReadPin(GPIO e_Gpio);
void LLD_GPIO_Toggle(GPIO e_Gpio);


#endif /* LLD_GPIO_H_ */
