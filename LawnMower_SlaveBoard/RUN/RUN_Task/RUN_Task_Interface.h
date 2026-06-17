/*
 * RUN_Task_Interface.h
 *
 *  Created on: 23 sept. 2022
 *      Author: morgan.venandy
 */

#ifndef RUN_RUN_TASK_RUN_TASK_INTERFACE_H_
#define RUN_RUN_TASK_RUN_TASK_INTERFACE_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdint.h>
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
#define NB_SLOT_CYCLIC_TASK 10u

#define CYCLIC_TASK_ADC_READ_VALUE	    0x00000001
#define CYCLIC_TASK_SONAR				0x00000040
#define CYCLIC_TASK_SEND_DATA   		0x00000800
#define CYCLIC_TASK_UPDATE_LED			0x00001000

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
void RUN_Task_Interface_Init(void);

#endif /* RUN_RUN_TASK_RUN_TASK_INTERFACE_H_ */
