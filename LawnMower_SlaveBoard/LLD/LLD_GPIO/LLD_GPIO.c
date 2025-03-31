/**
 * @file LLD_GPIO.c
 * @author ACR
 * @brief Specific GPIO driver
 * @details
**/

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdint.h>

#include "LLD_GPIO.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES GPIO ...                                                   */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* ... DATATYPES LLD GPIO ...                                               */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DEFINITIONS ...                                     */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... GLOBAL FUNCTIONS DEFINITIONS ...                                    */
/*--------------------------------------------------------------------------*/

/**
* @brief		GPIO initialization
* @details
**/
void LLD_GPIO_Init(void)
{
	 /***** PORT B *****/
	 DDRB = 0x00;
	 //DDRB |= (0<<DDB0) | (1<<DDB1) | (1<<DDB2) ; // Echo Sonar FC, FL, FR
	 DDRB |= (1<<DDB4) | (1<<DDB5) | (1<<DDB3); // Trigger Sonar FC, FL, FR
	 //DDRB |= (1<<DDB6); // XTAL
	 //DDRB |= (1<<DDB7); // XTAL
	 
	 PORTB = 0x00;
	 //PORTB |= (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2); // No Pull-Up Echo Sonar
	 // PORTB &= ~(1<<PORTB4) & ~(1<<PORTB5) & ~(1<<PORTB3); // Force à 0 Trigger
	 PORTB |= (1<<PORTB6) | (1<<PORTB7); // XTAL
	 
	 /***** PORT C *****/
	 DDRC = 0x00;
	 //DDRC |= (1<<DDC0) | (1<<DDC1) // ADC - Capteur Tension, courant
	 //DDRC |= (1<<DDC2); // Pluie - No Use
	 //DDRC |= (1<<DDC3); // SPARE ADC - No Use
	 //DDRC |= (1<<DDC4) | (1<<DDC5); // Config I2C SDA - SCL
	 //DDRC |= (1<<DDC6); // reset
	 
	 PORTC = 0x00;
	 //PORTC &= ~(1<<PORTC0) & ~(1<<PORTC1); // ADC - No Pull-Up
	 DDRC &= (1<<DDC2); // Pluie - No Use
	 PORTC |= (1<<PORTC3); // SPARE ADC - No Use
	 //PORTC &= ~(1<<PORTC4) & ~(1<<PORTC5); // I2C - Force à 0
	 PORTC |= (1<<PORTC6); // reset
	 
	 /***** PORT D *****/
	 DDRD = 0x00;
	 //DDRD |= (1<<DDD1); //| (0<<DDD0); // UART - TXD - RXD
	 DDRD |= (1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7); // LED GREEN - ORANGE - RED - YELLOW1 - 2 - 3
	 
	 PORTD = 0x00;
	 //PORTD |= (1<<PORTD0) | (1<<PORTD1); // UART - RX No Pull-Up - TX
	 //PORTD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7); // Force LED 0
 }

/**
* @brief		Sets the output level of GPIO pin to logic 1
* @param[in]	e_Gpio Name of GPIO pin to set the output level
* @return		void
* @details
**/
void LLD_GPIO_WritePin(GPIO e_Gpio)
{
	switch (e_Gpio)
	{
		case E_LEFT_TRIGGER:
			PORTB |= (1<<PORTB5);
			break;

		case E_CENTER_TRIGGER:
			PORTB |= (1<<PORTB4);
			break;

		case E_RIGHT_TRIGGER:
			PORTB |= (1<<PORTB3);
			break;

		case E_LED_GREEN:
			PORTD |= (1<<PORTD2);
			break;

		case E_LED_ORANGE:
			PORTD |= (1<<PORTD3);
			break;

		case E_LED_RED:
			PORTD |= (1<<PORTD4);
			break;

		case E_LED_YELLOW_1:
			PORTD |= (1<<PORTD5);
			break;

		case E_LED_YELLOW_2:
			PORTD |= (1<<PORTD6);
			break;

		case E_LED_YELLOW_3:
			PORTD |= (1<<PORTD7);
			break;
		
		default:
			break;
	}
}

/**
* @brief		Sets the output level of GPIO pin to logic 0
* @param[in]	e_Gpio Name of GPIO pin to set the output level
* @return		void
* @details
**/
void LLD_GPIO_ClearPin(GPIO e_Gpio)
{
	switch (e_Gpio)
	{
		case E_LEFT_TRIGGER:
			PORTB &= ~(1<<PORTB5);
			break;

		case E_CENTER_TRIGGER:
			PORTB &= ~(1<<PORTB4);
			break;

		case E_RIGHT_TRIGGER:
			PORTB &= ~(1<<PORTB3);
			break;

		case E_LED_GREEN:
			PORTD &= ~(1<<PORTD2);
			break;

		case E_LED_ORANGE:
			PORTD &= ~(1<<PORTD3);
			break;

		case E_LED_RED:
			PORTD &= ~(1<<PORTD4);
			break;

		case E_LED_YELLOW_1:
			PORTD &= ~(1<<PORTD5);
			break;

		case E_LED_YELLOW_2:
			PORTD &= ~(1<<PORTD6);
			break;

		case E_LED_YELLOW_3:
			PORTD &= ~(1<<PORTD7);
			break;
		
		default:
			break;
	}
}

/**
* @brief		Reads the current input value of the GPIO pin.
* @param[in]	e_Gpio Name of GPIO pin to read input value
* @return		Input value of GPIO pin
* @details
**/
uint8_t LLD_GPIO_ReadPin(GPIO e_Gpio)
{
	uint8_t u8_value = 0;
	switch (e_Gpio)
	{
		case E_LEFT_ECHO:
			u8_value = PINB & (1<<PINB1);
			break;

		case E_CENTER_ECHO:
			u8_value = PINB & (1<<PINB0);
			break;

		case E_RIGHT_ECHO:
			u8_value = PINB & (1<<PINB2);
			break;
		
		default:
			break;
	}
	return u8_value;
}

/**
* @brief		Reverses the current output logic of the GPIO pin
* @param[in]	e_Gpio Name of GPIO pin to reverse value
* @return		void
* @details
**/
void LLD_GPIO_Toggle(GPIO e_Gpio)
{
	
}
