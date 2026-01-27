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
	static char tc_uartBuffer[UART_BUFFER_SIZE] = {0};
	uint8_t u8_uartReturnState = 0;
	uint8_t u8_returnValue = 0;

	u8_uartReturnState = LLD_UART_GetBuffer(tc_uartBuffer);
	if (u8_uartReturnState != 0)
	{
		for (int i=0; i < UART_BUFFER_SIZE; i++)
		{
			*(pu8_RxBuffer+i) = tc_uartBuffer[i+1];
			if (*(pu8_RxBuffer+i) == '\r')
			{
				*(pu8_RxBuffer+(i+1)) = '\n';
				break;
			}
		}
		u8_returnValue = 1;
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
