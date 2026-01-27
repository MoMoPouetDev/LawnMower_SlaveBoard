/*
 * RUN_ADC.c
 *
 *  Created on: 23 sept. 2022
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "HAL_ADC.h"
#include "RUN_ADC.h"
#include "RUN_Sensors.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void RUN_ADC_Init()
{
	HAL_ADC_Init();
}

void RUN_ADC_ReadValue()
{
	uint8_t u8_value = 0;
	uint16_t u16_adcValue = 0;
	E_ADC_SENSOR e_adcSensor = E_ADC_SENSOR_BATT_A;

	u8_value = HAL_ADC_ReadValue(&e_adcSensor, &u16_adcValue);
	if (u8_value != 0)
	{
		if (e_adcSensor == E_ADC_SENSOR_BATT_V)
		{
			RUN_Sensors_GetBatteryPercent(u16_adcValue);
		}
		else if (e_adcSensor == E_ADC_SENSOR_BATT_A)
		{
			RUN_Sensors_IsCharging(u16_adcValue);
		}
	}
}

