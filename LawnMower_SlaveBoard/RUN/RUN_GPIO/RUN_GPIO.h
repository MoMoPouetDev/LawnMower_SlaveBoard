/*
 * RUN_GPIO.h
 *
 *  Created on: 23 sept. 2022
 *      Author: morgan.venandy
 */

#ifndef RUN_RUN_GPIO_RUN_GPIO_H_
#define RUN_RUN_GPIO_RUN_GPIO_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "stdint.h"
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
void RUN_GPIO_Init(void);
void RUN_GPIO_SetEtatMowerInCharge(void);
void RUN_GPIO_SetEtatMowerWaitingForMow(void);
void RUN_GPIO_SetEtatMowerInTask(void);
void RUN_GPIO_SetEtatMowerInWait(void);
void RUN_GPIO_SetEtatMowerReturnToBase(void);
void RUN_GPIO_SetErrorMowerNtr(void);
void RUN_GPIO_SetErrorMowerRain(void);
void RUN_GPIO_UpdateLed(void);
void RUN_GPIO_UpdateMowerState(uint8_t u8_ledState);

#endif /* RUN_RUN_GPIO_RUN_GPIO_H_ */
