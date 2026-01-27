/*
 * HAL_ADC.h
 *
 *  Created on: 17 août 2022
 *      Author: morgan.venandy
 */

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdint.h>

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
typedef enum 
{
	E_ADC_SENSOR_BATT_V = 0,
	E_ADC_SENSOR_BATT_A,

    E_ADC_SENSOR_NB
} E_ADC_SENSOR;
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
void HAL_ADC_Init(void);
uint8_t HAL_ADC_ReadValue(E_ADC_SENSOR* pe_adcSensor, uint16_t* pu16_adcValue);

#endif /* HAL_HAL_ADC_HAL_ADC_H_ */
