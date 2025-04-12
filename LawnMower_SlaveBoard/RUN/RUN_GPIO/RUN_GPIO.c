/*
 * RUN_GPIO.c
 *
 *  Created on: 23 sept. 2022
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "HAL_GPIO.h"
#include "RUN_GPIO.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
static EtatMower ge_etatMower;
static ErrorMower ge_errorMower;
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void RUN_GPIO_Init(void)
{
	HAL_GPIO_Init();

	ge_etatMower = UNKNOWN_ETAT;
	ge_errorMower = NTR;
}

void RUN_GPIO_SetEtatMowerInCharge(void)
{
	ge_etatMower = (EN_CHARGE);
}

void RUN_GPIO_SetEtatMowerWaitingForMow(void)
{
	ge_etatMower = (PAS_DE_TACHE_EN_COURS);
}

void RUN_GPIO_SetEtatMowerInTask(void)
{
	ge_etatMower = (TACHE_EN_COURS);
}

void RUN_GPIO_SetEtatMowerInWait(void)
{
	ge_etatMower = (PAUSE);
}

void RUN_GPIO_SetEtatMowerReturnToBase(void)
{
	ge_etatMower = (RETOUR_STATION);
}

void RUN_GPIO_SetErrorMowerNtr(void)
{
	ge_errorMower = (NTR);
}

void RUN_GPIO_SetErrorMowerRain(void)
{
	ge_errorMower = (DETECTED_RAIN);
}

void RUN_GPIO_UpdateLed(void)
{
	HAL_GPIO_UpdateLed(ge_etatMower, ge_errorMower);
}

void RUN_GPIO_UpdateMowerState(uint8_t u8_ledStatus)
{
	ge_etatMower = 0x0F & u8_ledStatus;
	ge_errorMower = 0xF0 & u8_ledStatus;
}
