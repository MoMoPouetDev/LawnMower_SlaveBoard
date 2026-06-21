/*
 * RUN_UART.c
 *
 *  Created on: 19 FEB 2023
 *      Author: morgan.venandy
 *
 *  Modifié : remplacement RUN_I2C → envoi capteurs vers Teensy 4.1 hub
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "HAL_UART.h"
#include "RUN_UART.h"
#include "RUN_Sensors.h"
#include "RUN_GPIO.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
static uint8_t gtu8_txFrame[SLAVE_TX_FRAME_LEN];
static uint8_t gu8_frameReady = 0u;

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/

/*
 * RUN_UART_Init
 */
void RUN_UART_Init(void)
{
    HAL_UART_Init();
}

void RUN_UART_Process(void)
{
	uint8_t u8_battVoltage = 0;
	uint8_t u8_battAmp = 0;
	uint8_t u8_dock = 0;
	uint8_t u8_sonarFC = 0;
	uint8_t u8_sonarFL = 0;
	uint8_t u8_sonarFR = 0;

	u8_battVoltage = RUN_Sensors_GetBatteryPercent();
	u8_battAmp = RUN_Sensors_IsCharging();
	u8_dock = RUN_Sensors_GetDockState();
	u8_sonarFC = RUN_Sensors_SonarGetDistanceFC();
	u8_sonarFL = RUN_Sensors_SonarGetDistanceFL();
	u8_sonarFR = RUN_Sensors_SonarGetDistanceFR();

	RUN_UART_SendSensors(u8_battVoltage, u8_battAmp, u8_dock, u8_sonarFC, u8_sonarFL, u8_sonarFR);
	RUN_UART_Receive();
}

void RUN_UART_Receive(void)
{
	static uint8_t _tu8_uartRxBuffer[2] = { 0 };
	uint8_t u8_returnValueUART = 0;

	u8_returnValueUART = HAL_UART_ReceiveCommand(_tu8_uartRxBuffer, 2);
	if ((u8_returnValueUART != 0) && (_tu8_uartRxBuffer[0] == 0x00))
	{
		RUN_GPIO_UpdateMowerState(_tu8_uartRxBuffer[1]);
	}
}

/*
 * RUN_UART_SendSensors
 *
 * Remplace la réponse I2C slave (TWI_sendData).
 * À appeler périodiquement depuis le scheduler Timer (toutes les ~100ms).
 *
 * Premier appel du cycle (gu8_frameReady == 0) :
 *   → Construit la trame avec les données capteurs fraîches.
 * Appels suivants :
 *   → Continue l'envoi octet par octet via HAL_UART_SendCommand()
 *     (même mécanique non-bloquante que ton existant).
 * Retourne 1 quand les 8 octets ont tous été envoyés.
 *
 * Appel depuis le scheduler :
 *   RUN_UART_SendSensors(
 *       gu8_batteryVoltage, gu8_batteryAmp, ge_dock,
 *       gu8_distanceSonarFC, gu8_distanceSonarFL, gu8_distanceSonarFR);
 */
uint8_t RUN_UART_SendSensors(uint8_t u8_battVoltage,
                             uint8_t u8_battAmp,
                             uint8_t u8_dock,
                             uint8_t u8_sonarFC,
                             uint8_t u8_sonarFL,
                             uint8_t u8_sonarFR)
{
    uint8_t u8_returnValue = 0u;

    if (gu8_frameReady == 0u)
    {
        gtu8_txFrame[0u] = SLAVE_TX_SOF;
        gtu8_txFrame[1u] = u8_battVoltage;
        gtu8_txFrame[2u] = u8_battAmp;
        gtu8_txFrame[3u] = u8_dock;
        gtu8_txFrame[4u] = u8_sonarFC;
        gtu8_txFrame[5u] = u8_sonarFL;
        gtu8_txFrame[6u] = u8_sonarFR;
        /* XOR des données uniquement, le SOF est exclu */
        gtu8_txFrame[7u] = gtu8_txFrame[1u]
                         ^ gtu8_txFrame[2u]
                         ^ gtu8_txFrame[3u]
                         ^ gtu8_txFrame[4u]
                         ^ gtu8_txFrame[5u]
                         ^ gtu8_txFrame[6u];
        gu8_frameReady = 1u;
    }

    if (HAL_UART_SendCommand(gtu8_txFrame, SLAVE_TX_FRAME_LEN) != 0u)
    {
        gu8_frameReady = 0u;
        u8_returnValue = 1u;
    }

    return u8_returnValue;
}