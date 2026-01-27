/*
 * HAL_ADC.c
 *
 *  Created on: 17 août 2022
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdint.h>

#include "LLD_ADC.h"
#include "HAL_ADC.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
/*** Convertisseur Analogique Numérique ***/
#define PIN_ADC0_BATTERY_VOLTAGE 0
#define PIN_ADC1_BATTERY_AMPERAGE 1
/*** END ***/

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void HAL_ADC_Init(void)
{
    /* Test ADC */
	LLD_ADC_Init();
}

uint8_t HAL_ADC_ReadValue(E_ADC_SENSOR* pe_adcSensor, uint16_t* pu16_adcValue)
{
	static uint8_t _u8_adcState = 0;
	uint8_t u8_adcReturn = 0;
	uint8_t u8_returnValue = 0;
	uint16_t u16_adcValue = 0;

	switch (_u8_adcState)
	{
	case 0:
		u8_adcReturn = LLD_ADC_ReadConversionValue(PIN_ADC0_BATTERY_VOLTAGE, &u16_adcValue);
		if (u8_adcReturn != 0)
		{
			*pu16_adcValue = u16_adcValue;
			_u8_adcState = 1;
			u8_returnValue = 1;
			*pe_adcSensor = E_ADC_SENSOR_BATT_V;
		}
		break;
	
	case 1:
		u8_adcReturn = LLD_ADC_ReadConversionValue(PIN_ADC1_BATTERY_AMPERAGE, &u16_adcValue);
		if (u8_adcReturn != 0)
		{
			*pu16_adcValue = u16_adcValue;
			_u8_adcState = 0;
			u8_returnValue = 1;
			*pe_adcSensor = E_ADC_SENSOR_BATT_A;
		}
		break;
	
	default:
		break;
	}

	return u8_returnValue;
}
