/*
 * FSM_Main.c
 *
 *  Created on: 7 sept. 2022
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                         */
/*--------------------------------------------------------------------------*/
#include "RUN_Init.h"
#include "RUN_Task.h"
#include "RUN_Task_Interface.h"

#include "FSM_Enum.h"
#include "FSM_Init.h"
#include "FSM_Operative.h"
#include "FSM_Main.h"

/*--------------------------------------------------------------------------*/
/* ... DATAS TYPE ...                                                       */
/*--------------------------------------------------------------------------*/
S_MOWER_FSM_STATE ge_FSM_Phase;
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* ... FUNCTIONS DEFINITIONS...                                              */
/*---------------------------------------------------------------------------*/
/**
 * \fn void FSM_Main_Init( void )
 * \brief This function initialize all finite state machine
 * \details - Initialize Event finite state machine
 * - by calling Events_Init()
 * - by calling FSM_Init_Init()
 * - by calling FSM_StopButton_Init()
**/
void FSM_Main_Init( void )
{
	/*** FSM Init ***/
	ge_FSM_Phase = FSM_Enum_GetFsmPhase();
	FSM_Init_Init();
	FSM_Operative_Init();

	/*** RUN Init ***/
	RUN_Init();
}

void FSM_Main( void )
{
   while(1)
   {
		uint32_t u32_CyclicTask;
	/***************************************************************************************************************/
	/*                                      MANAGE RUN TASK CYCLE                                                  */
	/***************************************************************************************************************/

		u32_CyclicTask = RUN_Task_GetCyclicTask();
		
      /***************************************************************************************************************/
      /*                                  DEBUG                                                   */
      /***************************************************************************************************************/
		//FSM_TEST_SonarDistance(u32_CyclicTask);

	  /***************************************************************************************************************/
	  /*                                   FINITE STATE MACHINE                                                      */
	  /***************************************************************************************************************/

		if (ge_FSM_Phase == PHASE_INIT_INIT)
		{
			FSM_Init( ge_FSM_Phase );
		}
		else if ( (ge_FSM_Phase >= PHASE_OPERATIVE_INIT) && (ge_FSM_Phase <= PHASE_OPERATIVE_WAITING_FOR_RETURN_TO_BASE) )
		{
			FSM_Operative( ge_FSM_Phase );
		}
		FSM_Main_UpdateFsmMower();
   }
}

void FSM_Main_UpdateFsmMower()
{
	ge_FSM_Phase = FSM_Enum_GetFsmPhase();
}
