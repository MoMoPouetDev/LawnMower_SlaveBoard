/*
 * RUN_UART.h
 *
 *  Created on: 19 FEB 2023
 *      Author: morgan.venandy
 *
 *  Modifié : remplacement RUN_I2C → envoi capteurs vers Teensy 4.1 hub
 */

#ifndef RUN_RUN_UART_RUN_UART_H_
#define RUN_RUN_UART_RUN_UART_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "stdint.h"

/*--------------------------------------------------------------------------*/
/* ... DEFINES TRAME SLAVE → TEENSY ...                                    */
/*--------------------------------------------------------------------------*/

/*
 * Trame envoyée vers le Teensy (8 octets) :
 * [0xBB][BAT_V][BAT_A][DOCK][FC][FL][FR][XOR]
 *   0      1     2      3    4   5   6    7
 * XOR calculé sur octets [1..6]
 */
#define SLAVE_TX_SOF        0xBBu
#define SLAVE_TX_FRAME_LEN  8u

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DECLARATIONS ...                                          */
/*--------------------------------------------------------------------------*/

void RUN_UART_Init(void);
void RUN_UART_Process();
void RUN_UART_Receive();
/* Nouveau — envoi trame capteurs vers Teensy (remplace réponse I2C slave) */
uint8_t RUN_UART_SendSensors(uint8_t u8_battVoltage,
                             uint8_t u8_battAmp,
                             uint8_t u8_dock,
                             uint8_t u8_sonarFC,
                             uint8_t u8_sonarFL,
                             uint8_t u8_sonarFR);

#endif /* RUN_RUN_UART_RUN_UART_H_ */