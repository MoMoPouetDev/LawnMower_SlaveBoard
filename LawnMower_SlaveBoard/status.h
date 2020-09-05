//
//  status.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 05/09/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef status_h
#define status_h

#include "constant.h"

typedef enum {
    UNKNOWN_ETAT = 0x00,
    TACHE_EN_COURS = 0x01,
    RETOUR_STATION = 0x02,
    EN_CHARGE = 0x03,
    PAS_DE_TACHE_EN_COURS = 0x04,
    PAUSE = 0x05
}EtatMower;

typedef enum {
    NTR = 0x10,
    BLOCKED_MOWER = 0x20,
    DETECTED_RAIN = 0x30,
    WIRE_NOT_DETECTED = 0x40,
    LOW_BATTERY = 0x50,
    VERY_LOW_BATTERY = 0x60,
    EMPTY_BATTERY = 0x70
}ErrorMower;

void STATUS_updateStatus(void);
void STATUS_updateLedStatus(EtatMower*);
void STATUS_updateLedError(ErrorMower*);
void STATUS_updateLed(EtatMower*, ErrorMower*);

#endif /* status_h */
