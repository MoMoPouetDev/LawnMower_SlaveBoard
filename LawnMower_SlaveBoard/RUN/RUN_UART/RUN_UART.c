/*
 * RUN_UART.c
 *
 *  Created on: 19 FEB 2023
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "RUN_UART.h"
#include "RUN_Sensors.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static void _RUN_UART_UintToString(uint8_t u8_value, char* pc_buffer);
static void _RUN_UART_Uint32ToStringPadded(uint32_t u32_value, uint8_t u8_digits, char* pc_buffer);

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void RUN_UART_Init()
{
	HAL_UART_Init();
}

void RUN_UART_DebugSendStatus(void)
{
	static uint16_t _u16_slaveCpt = 0;
#ifdef DEBUG_UART
	if (_u16_slaveCpt >= 100)
	{
		static char tc_txBuffer[48] = {0};
		uint8_t u8_distanceSonarFC = 0;
		uint8_t u8_distanceSonarFL = 0;
		uint8_t u8_distanceSonarFR = 0;
		char tc_buffer[8] = {0};


		u8_distanceSonarFC = RUN_Sensors_SonarGetDistanceFC();
		u8_distanceSonarFL = RUN_Sensors_SonarGetDistanceFL();
		u8_distanceSonarFR = RUN_Sensors_SonarGetDistanceFR();

		/* Build string manuellement */
		tc_txBuffer[0] = '\0';

		strcat(tc_txBuffer, "SONAR_FC=");
		_RUN_UART_UintToString(u8_distanceSonarFC, tc_buffer);
		strcat(tc_txBuffer, tc_buffer);

		strcat(tc_txBuffer, ";SONAR_FL=");
		_RUN_UART_UintToString(u8_distanceSonarFL, tc_buffer);
		strcat(tc_txBuffer, tc_buffer);

		strcat(tc_txBuffer, ";SONAR_FR=");
		_RUN_UART_UintToString(u8_distanceSonarFR, tc_buffer);
		strcat(tc_txBuffer, tc_buffer);

		strcat(tc_txBuffer, "\r\n");

		HAL_UART_SendString(tc_txBuffer);

		_u16_slaveCpt = 0;
	}
	else
	{
		_u16_slaveCpt++;
	}
#else
    return;
#endif
}

static void _RUN_UART_UintToString(uint8_t u8_value, char* pc_buffer)
{
    uint8_t u8_temp = u8_value;
    uint8_t u8_digits = 0;
    uint8_t u8_result[4] = {0};
    uint8_t u8_i = 0;

    if (u8_temp == 0)
    {
        pc_buffer[0] = '0';
        pc_buffer[1] = '\0';
        return;
    }

    while (u8_temp > 0)
    {
        u8_result[u8_digits++] = (uint8_t)(u8_temp % 10);
        u8_temp = (uint8_t)(u8_temp / 10);
    }

    for (u8_i = 0; u8_i < u8_digits; u8_i++)
    {
        pc_buffer[u8_i] = (char)(u8_result[u8_digits - u8_i - 1] + '0');
    }
    pc_buffer[u8_digits] = '\0';
}

static void _RUN_UART_Uint32ToStringPadded(uint32_t u32_value, uint8_t u8_digits, char* pc_buffer)
{
    uint32_t u32_temp = u32_value;
    uint8_t u8_i = 0;

    for (u8_i = 0; u8_i < u8_digits; u8_i++)
    {
        pc_buffer[u8_digits - u8_i - 1] = (char)('0' + (u32_temp % 10));
        u32_temp /= 10;
    }
    pc_buffer[u8_digits] = '\0';
}
