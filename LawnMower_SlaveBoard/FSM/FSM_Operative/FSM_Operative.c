/*
 * FSM_Operative.c
 *
 *  Created on: 23 sept. 2022
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include "RUN_Task_Interface.h"
#include "RUN_Task.h"
#include "RUN_ADC.h"
#include "RUN_GPIO.h"
#include "RUN_Sensors.h"
#include "RUN_GPS.h"
#include "RUN_I2C.h"
#include "RUN_UART.h"
#include "FSM_Enum.h"
#include "FSM_Operative.h"

/*--------------------------------------------------------------------------*/
/* ... DATAS TYPE ...                                                       */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static void _FSM_Operative_SonarDistance(uint32_t u32_CyclicTask);
static void _FSM_Operative_GpsAcquisition(uint32_t u32_CyclicTask);
static void _FSM_Operative_UpdateLed(uint32_t u32_CyclicTask);
static void _FSM_Operative_ADCRead(uint32_t u32_CyclicTask);
/*---------------------------------------------------------------------------*/
/* ... FUNCTIONS DEFINITIONS...                                              */
/*---------------------------------------------------------------------------*/
void FSM_Operative_Init()
{

}

void FSM_Operative(S_MOWER_FSM_STATE e_FSM_Operative_State)
{
	uint8_t u8_isI2cBusy = 0;
	uint32_t u32_CyclicTask;
	/***************************************************************************************************************/
	/*                                      MANAGE RUN TASK CYCLE                                                  */
	/***************************************************************************************************************/
	u32_CyclicTask = RUN_Task_GetCyclicTask();

	/***************************************************************************************************************/
	/*                                  ACU FINITE STATE MACHINE                                                   */
	/***************************************************************************************************************/

	switch( e_FSM_Operative_State )
   	{
	  	default:
	  	case S_SUP_OPERATIVE_Init:
		 	FSM_Operative_Init();
			FSM_Enum_SetFsmPhase(S_SUP_OPERATIVE_Moving);
			break;

	  	case S_SUP_OPERATIVE_Moving :
			_FSM_Operative_SonarDistance(u32_CyclicTask);
			_FSM_Operative_GpsAcquisition(u32_CyclicTask);
			_FSM_Operative_UpdateLed(u32_CyclicTask);
			_FSM_Operative_ADCRead(u32_CyclicTask);
			break;
   	}
}

static void _FSM_Operative_SonarDistance(uint32_t u32_CyclicTask)
{
	uint8_t u8_flagClear = 0;
	if ( (u32_CyclicTask & CYCLIC_TASK_SONAR) != 0) {
		u8_flagClear = RUN_Sensors_SonarDistance();
		if (u8_flagClear != 0)
		{
			RUN_Task_EraseCyclicTask(CYCLIC_TASK_SONAR);
		}
	}
}

static void _FSM_Operative_GpsAcquisition(uint32_t u32_CyclicTask)
{
	uint8_t u8_flagClear = 0;
	if ( (u32_CyclicTask & CYCLIC_TASK_GPS_ACQUISITION) != 0) {
#ifndef DEBUG_UART
		u8_flagClear = RUN_GPS_startGpsAcquisition();
		if (u8_flagClear != 0)
		{
			RUN_Task_EraseCyclicTask(CYCLIC_TASK_GPS_ACQUISITION);
		}
#else
		RUN_UART_DebugSendStatus();
		RUN_Task_EraseCyclicTask(CYCLIC_TASK_GPS_ACQUISITION);
#endif
	}
}

static void _FSM_Operative_UpdateLed(uint32_t u32_CyclicTask)
{
	if ( (u32_CyclicTask & CYCLIC_TASK_UPDATE_LED) != 0) {
		RUN_GPIO_UpdateLed();
		RUN_Task_EraseCyclicTask(CYCLIC_TASK_UPDATE_LED);
	}
}
static void _FSM_Operative_ADCRead(uint32_t u32_CyclicTask)
{
	if ( (u32_CyclicTask & CYCLIC_TASK_ADC_READ_VALUE) != 0) {
		RUN_ADC_ReadValue();
		RUN_Task_EraseCyclicTask(CYCLIC_TASK_ADC_READ_VALUE);
	}
}
