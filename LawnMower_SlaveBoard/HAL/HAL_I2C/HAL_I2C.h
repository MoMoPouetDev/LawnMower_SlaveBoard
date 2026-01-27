/*
 * HAL_I2C.h
 *
 *  Created on: 12 sept. 2022
 *      Author: morgan.venandy
 */

#ifndef HAL_HAL_I2C_HAL_I2C_H_
#define HAL_HAL_I2C_HAL_I2C_H_

#include <stdint.h>
#include "LLD_I2C.h"

void HAL_I2C_Init(void);
uint8_t HAL_I2C_IsI2cBusy(void);

#endif /* HAL_HAL_I2C_HAL_I2C_H_ */
