/*
 * RUN_I2C.h
 *
 *  Created on: 12 FEB 2023
 *      Author: morgan.venandy
 */

#ifndef RUN_RUN_I2C_RUN_I2C_H_
#define RUN_RUN_I2C_RUN_I2C_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "stdint.h"
#include "HAL_I2C.h"
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
void RUN_I2C_Init(void);
uint8_t* RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA e_slaveReadData);
uint8_t RUN_I2C_IsI2cBusy(void);

#endif /* RUN_RUN_I2C_RUN_I2C_H_ */
