/*
 * HAL_GPIO.c
 *
 *  Created on: 19 août 2022
 *      Author: morgan.venandy
 */


/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdint.h>

#include "LLD_GPIO.h"
#include "HAL_GPIO.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void HAL_GPIO_Init()
{
	LLD_GPIO_Init();
}

void HAL_GPIO_UpdateLed(EtatMower e_etatMower, ErrorMower e_errorMower) 
{
    switch(e_etatMower) {
        case UNKNOWN_ETAT:
			LLD_GPIO_ClearPin(E_GREEN_LED);
			LLD_GPIO_ClearPin(E_ORANGE_LED);
			LLD_GPIO_ClearPin(E_RED_LED);
            break;
            
        case TACHE_EN_COURS:
			LLD_GPIO_WritePin(E_GREEN_LED);
			LLD_GPIO_ClearPin(E_ORANGE_LED);
			LLD_GPIO_ClearPin(E_RED_LED);
            break;
            
        case RETOUR_STATION:
			LLD_GPIO_ClearPin(E_GREEN_LED);
			LLD_GPIO_WritePin(E_ORANGE_LED);
			LLD_GPIO_ClearPin(E_RED_LED);
            break;
            
        case EN_CHARGE:
			LLD_GPIO_WritePin(E_GREEN_LED);
			LLD_GPIO_WritePin(E_ORANGE_LED);
			LLD_GPIO_WritePin(E_RED_LED);
            break;
            
        case PAS_DE_TACHE_EN_COURS:
			LLD_GPIO_ClearPin(E_GREEN_LED);
			LLD_GPIO_ClearPin(E_ORANGE_LED);
			LLD_GPIO_WritePin(E_RED_LED);
            break;
            
        case PAUSE:
			LLD_GPIO_WritePin(E_GREEN_LED);
			LLD_GPIO_WritePin(E_ORANGE_LED);
			LLD_GPIO_ClearPin(E_RED_LED);
            break;
            
        default:
			LLD_GPIO_ClearPin(E_GREEN_LED);
			LLD_GPIO_ClearPin(E_ORANGE_LED);
			LLD_GPIO_ClearPin(E_RED_LED);
            break;
    }
    
    switch(e_errorMower) {
        case NTR:
			LLD_GPIO_ClearPin(E_YELLOW_ONE_LED);
			LLD_GPIO_ClearPin(E_YELLOW_TWO_LED);
			LLD_GPIO_ClearPin(E_YELLOW_THREE_LED);
            break;
            
        case BLOCKED_MOWER:
			LLD_GPIO_ClearPin(E_YELLOW_ONE_LED);
			LLD_GPIO_ClearPin(E_YELLOW_TWO_LED);
			LLD_GPIO_WritePin(E_YELLOW_THREE_LED);
            break;
            
        case DETECTED_RAIN:
			LLD_GPIO_ClearPin(E_YELLOW_ONE_LED);
			LLD_GPIO_WritePin(E_YELLOW_TWO_LED);
			LLD_GPIO_ClearPin(E_YELLOW_THREE_LED);
            break;
            
        case WIRE_NOT_DETECTED:
			LLD_GPIO_ClearPin(E_YELLOW_ONE_LED);
			LLD_GPIO_WritePin(E_YELLOW_TWO_LED);
			LLD_GPIO_WritePin(E_YELLOW_THREE_LED);
            break;
            
        case LOW_BATTERY:
			LLD_GPIO_WritePin(E_YELLOW_ONE_LED);
			LLD_GPIO_ClearPin(E_YELLOW_TWO_LED);
			LLD_GPIO_ClearPin(E_YELLOW_THREE_LED);
            break;
            
        case VERY_LOW_BATTERY:
			LLD_GPIO_WritePin(E_YELLOW_ONE_LED);
			LLD_GPIO_ClearPin(E_YELLOW_TWO_LED);
			LLD_GPIO_WritePin(E_YELLOW_THREE_LED);
            break;
            
        case EMPTY_BATTERY:
			LLD_GPIO_WritePin(E_YELLOW_ONE_LED);
			LLD_GPIO_WritePin(E_YELLOW_TWO_LED);
			LLD_GPIO_ClearPin(E_YELLOW_THREE_LED);
            break;
            
        default:
			LLD_GPIO_ClearPin(E_YELLOW_ONE_LED);
			LLD_GPIO_ClearPin(E_YELLOW_TWO_LED);
			LLD_GPIO_ClearPin(E_YELLOW_THREE_LED);
            break;
    }
}

void HAL_GPIO_WritePinSolar(GPIO e_gpio, uint8_t u8_value)
{
	if (u8_value == 0)
	{
		LLD_GPIO_ClearPin(e_gpio);
	}
	else
	{
		LLD_GPIO_WritePin(e_gpio);
	}
}

uint8_t HAL_GPIO_ReadPinSolar(GPIO e_gpio)
{
	return LLD_GPIO_ReadPin(e_gpio);
}
