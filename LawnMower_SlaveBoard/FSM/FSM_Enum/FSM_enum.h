#ifndef _FSM_ENUM_H_
#define _FSM_ENUM_H_

#define PHASE_INIT_INIT                                             0x00

#define PHASE_OPERATIVE_INIT                                        0x10   //16
#define PHASE_OPERATIVE_WAITING_FOR_RETURN_TO_BASE                  0x1F   //31

/* DECLARATION DES ETATS DU MODULE SUPERVISOR */
 typedef enum
 {
     S_SUP_INIT_Init                                = PHASE_INIT_INIT,

     S_SUP_OPERATIVE_Init                           = PHASE_OPERATIVE_INIT,//64
     S_SUP_OPERATIVE_Moving                         = PHASE_OPERATIVE_WAITING_FOR_RETURN_TO_BASE

 }S_MOWER_FSM_STATE;

void FSM_Enum_SetFsmPhase(S_MOWER_FSM_STATE _FSM_Phase);
S_MOWER_FSM_STATE FSM_Enum_GetFsmPhase(void);

 #endif /* _FSM_ENUM_H_ */
