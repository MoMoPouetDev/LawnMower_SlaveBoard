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
			LLD_GPIO_ClearPin(E_LED_GREEN);
			LLD_GPIO_ClearPin(E_LED_ORANGE);
			LLD_GPIO_ClearPin(E_LED_RED);
            break;
            
        case TACHE_EN_COURS:
			LLD_GPIO_WritePin(E_LED_GREEN);
			LLD_GPIO_ClearPin(E_LED_ORANGE);
			LLD_GPIO_ClearPin(E_LED_RED);
            break;
            
        case RETOUR_STATION:
			LLD_GPIO_ClearPin(E_LED_GREEN);
			LLD_GPIO_WritePin(E_LED_ORANGE);
			LLD_GPIO_ClearPin(E_LED_RED);
            break;
            
        case EN_CHARGE:
			LLD_GPIO_WritePin(E_LED_GREEN);
			LLD_GPIO_WritePin(E_LED_ORANGE);
			LLD_GPIO_WritePin(E_LED_RED);
            break;
            
        case PAS_DE_TACHE_EN_COURS:
			LLD_GPIO_ClearPin(E_LED_GREEN);
			LLD_GPIO_ClearPin(E_LED_ORANGE);
			LLD_GPIO_WritePin(E_LED_RED);
            break;
            
        case PAUSE:
			LLD_GPIO_WritePin(E_LED_GREEN);
			LLD_GPIO_WritePin(E_LED_ORANGE);
			LLD_GPIO_ClearPin(E_LED_RED);
            break;
            
        default:
			LLD_GPIO_ClearPin(E_LED_GREEN);
			LLD_GPIO_ClearPin(E_LED_ORANGE);
			LLD_GPIO_ClearPin(E_LED_RED);
            break;
    }
    
    switch(e_errorMower) {
        case NTR:
			LLD_GPIO_ClearPin(E_LED_YELLOW_1);
			LLD_GPIO_ClearPin(E_LED_YELLOW_2);
			LLD_GPIO_ClearPin(E_LED_YELLOW_3);
            break;
            
        case BLOCKED_MOWER:
			LLD_GPIO_ClearPin(E_LED_YELLOW_1);
			LLD_GPIO_ClearPin(E_LED_YELLOW_2);
			LLD_GPIO_WritePin(E_LED_YELLOW_3);
            break;
            
        case DETECTED_RAIN:
			LLD_GPIO_ClearPin(E_LED_YELLOW_1);
			LLD_GPIO_WritePin(E_LED_YELLOW_2);
			LLD_GPIO_ClearPin(E_LED_YELLOW_3);
            break;
            
        case WIRE_NOT_DETECTED:
			LLD_GPIO_ClearPin(E_LED_YELLOW_1);
			LLD_GPIO_WritePin(E_LED_YELLOW_2);
			LLD_GPIO_WritePin(E_LED_YELLOW_3);
            break;
            
        case LOW_BATTERY:
			LLD_GPIO_WritePin(E_LED_YELLOW_1);
			LLD_GPIO_ClearPin(E_LED_YELLOW_2);
			LLD_GPIO_ClearPin(E_LED_YELLOW_3);
            break;
            
        case VERY_LOW_BATTERY:
			LLD_GPIO_WritePin(E_LED_YELLOW_1);
			LLD_GPIO_ClearPin(E_LED_YELLOW_2);
			LLD_GPIO_WritePin(E_LED_YELLOW_3);
            break;
            
        case EMPTY_BATTERY:
			LLD_GPIO_WritePin(E_LED_YELLOW_1);
			LLD_GPIO_WritePin(E_LED_YELLOW_2);
			LLD_GPIO_ClearPin(E_LED_YELLOW_3);
            break;
            
        default:
			LLD_GPIO_ClearPin(E_LED_YELLOW_1);
			LLD_GPIO_ClearPin(E_LED_YELLOW_2);
			LLD_GPIO_ClearPin(E_LED_YELLOW_3);
            break;
    }
}

void HAL_GPIO_WritePinSonar(GPIO e_gpio, uint8_t u8_value)
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

uint8_t HAL_GPIO_ReadPinSonar(GPIO e_gpio)
{
	return LLD_GPIO_ReadPin(e_gpio);
}
