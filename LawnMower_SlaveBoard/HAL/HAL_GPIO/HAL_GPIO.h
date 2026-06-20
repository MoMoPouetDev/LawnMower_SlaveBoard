/*
 * HAL_GPIO.h
 *
 *  Created on: 19 août 2022
 *      Author: morgan.venandy
 */

#ifndef HAL_HAL_GPIO_HAL_GPIO_H_
#define HAL_HAL_GPIO_HAL_GPIO_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "stdint.h"
#include "LLD_GPIO.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
typedef enum {
    UNKNOWN_ETAT = 0x00,
    TACHE_EN_COURS = 0x01,
    RETOUR_STATION = 0x02,
    EN_CHARGE = 0x03,
    PAS_DE_TACHE_EN_COURS = 0x04,
    PAUSE = 0x05
}EtatMower;

typedef enum {
    NTR = 0x10,
    BLOCKED_MOWER = 0x20,
    DETECTED_RAIN = 0x30,
    WIRE_NOT_DETECTED = 0x40,
    LOW_BATTERY = 0x50,
    VERY_LOW_BATTERY = 0x60,
    EMPTY_BATTERY = 0x70
}ErrorMower;

typedef enum
{
    OFF, 
	ON
}Etat;

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
void HAL_GPIO_Init(void);
void HAL_GPIO_UpdateLed(EtatMower e_etatMower, ErrorMower e_errorMower);
void HAL_GPIO_WritePinSonar(GPIO e_gpio, uint8_t u8_value);
uint8_t HAL_GPIO_ReadPinSonar(GPIO e_gpio);

#endif /* HAL_HAL_GPIO_HAL_GPIO_H_ */
