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

static uint16_t gu16_adcValueBattVolt;
static uint16_t gu16_adcValueBattAmp;

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void HAL_ADC_Init(void)
{
  	gu16_adcValueBattVolt = 0;
  	gu16_adcValueBattAmp = 0;

    /* Test ADC */
	LLD_ADC_Init();
}

void HAL_ADC_ReadValue(void)
{
	uint8_t u8_adcReturn = 0;
	uint8_t u8_adcState = 0;
	uint16_t u16_adcValue = 0;

	switch (u8_adcState)
	{
	case 0:
		u8_adcReturn = LLD_ADC_ReadConversionValue(PIN_ADC0_BATTERY_VOLTAGE, &u16_adcValue);
		if (u8_adcReturn != 0)
		{
			gu16_adcValueBattVolt = u16_adcValue;
			u8_adcState = 1;
		}
		break;
	
	case 1:
		u8_adcReturn = LLD_ADC_ReadConversionValue(PIN_ADC1_BATTERY_AMPERAGE, &u16_adcValue);
		if (u8_adcReturn != 0)
		{
			gu16_adcValueBattAmp = u16_adcValue;
			u8_adcState = 0;
		}
		break;
	
	default:
		break;
	}
}

uint16_t HAL_ADC_GetChargeValue(void)
{
	return gu16_adcValueBattAmp;
}

uint16_t HAL_ADC_GetBatteryValue(void)
{
	return gu16_adcValueBattVolt;
}
