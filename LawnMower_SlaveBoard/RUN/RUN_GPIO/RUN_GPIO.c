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
void RUN_GPIO_Init()
{
	HAL_GPIO_Init();

	ge_etatMower = UNKNOWN_ETAT;
	ge_errorMower = NTR;
}

void RUN_GPIO_SetEtatMowerInCharge()
{
	ge_etatMower = (EN_CHARGE);
}

void RUN_GPIO_SetEtatMowerWaitingForMow()
{
	ge_etatMower = (PAS_DE_TACHE_EN_COURS);
}

void RUN_GPIO_SetEtatMowerInTask()
{
	ge_etatMower = (TACHE_EN_COURS);
}

void RUN_GPIO_SetEtatMowerInWait()
{
	ge_etatMower = (PAUSE);
}

void RUN_GPIO_SetEtatMowerReturnToBase()
{
	ge_etatMower = (RETOUR_STATION);
}

void RUN_GPIO_SetErrorMowerNtr()
{
	ge_errorMower = (NTR);
}

void RUN_GPIO_SetErrorMowerRain()
{
	ge_errorMower = (DETECTED_RAIN);
}

void RUN_GPIO_UpdateLed()
{
	HAL_GPIO_UpdateLed(ge_etatMower, ge_errorMower);
}
