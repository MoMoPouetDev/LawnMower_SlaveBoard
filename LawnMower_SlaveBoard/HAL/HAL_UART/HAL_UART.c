/*
 * HAL_UART.c
 *
 *  Created on: 19 FEB 2023
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "LLD_UART.h"
#include "HAL_UART.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void HAL_UART_Init()
{
	LLD_UART_Init();
}

uint8_t HAL_UART_ReceiveCommand(uint8_t* pu8_RxBuffer, uint8_t u8_size)
{
	static uint8_t u8_uartState = 0;
	static uint8_t u8_ind = 0;
	uint8_t u8_uartReturnState = 0;
	uint8_t u8_returnValue = 0;

	switch (u8_uartState)
	{
		case 0:
			u8_uartReturnState = LLD_UART_Receive(pu8_RxBuffer + u8_ind);
			if (u8_uartReturnState != 0)
			{
				if (u8_ind < (u8_size-1))
				{
					u8_size++;
				}
				else
				{
					u8_uartState++;
				}
			}
			break;

		case 1:
			u8_uartState = 0;
			u8_returnValue = 1;
			break;

		default:
			u8_uartState = 0;
			break;
	}
	return u8_returnValue;
}

uint8_t HAL_UART_SendCommand(uint8_t* pu8_buffer, uint8_t u8_bufferSize)
{
	static uint8_t u8_i = 0;
	uint8_t u8_returnValue = 0;
	
	if (u8_i < u8_bufferSize)
	{
		LLD_UART_Send(pu8_buffer + u8_i);
		u8_i++;
	}
	else
	{
		u8_i = 0;
		u8_returnValue = 1;
	}

	return u8_returnValue;
}
