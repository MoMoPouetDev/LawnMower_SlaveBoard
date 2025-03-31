/*
 * RUN_Task_Interface.c
 *
 *  Created on: 5 oct. 2022
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include "RUN_Task.h"
#include "RUN_Task_Interface.h"

/*--------------------------------------------------------------------------*/
/* ... DATAS TYPE ...                                                       */
/*--------------------------------------------------------------------------*/
static uint32_t gArraySlotTask[NB_SLOT_CYCLIC_TASK] =
{
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR | CYCLIC_TASK_ADC_READ_VALUE,// 0
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR,  // 1
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR | CYCLIC_TASK_UPDATE_LED, // 2
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR, // 3
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR, // 4
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR,  // 5
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR,  // 6
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR,  // 7
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR,  // 8
	CYCLIC_TASK_GPS_ACQUISITION | CYCLIC_TASK_SONAR,  // 9
};

/*--------------------------------------------------------------------------*/
/* ... DATAS ...                                                            */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* ... LOCAL FUNCTIONS DECLARATIONS ...                                     */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* ... FUNCTIONS IMPLEMENTATIONS...                                         */
/*--------------------------------------------------------------------------*/
void RUN_Task_Interface_Init()
{
	RUN_Task_Sequencer_Init();
	RUN_Task_SetArraySlotTask(gArraySlotTask, NB_SLOT_CYCLIC_TASK);
}

