//
//  status.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 05/09/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include "status.h"
#include <util/delay.h>

void STATUS_updateStatus(uint8_t dataTWI) {
    EtatMower eEtatMower;
    ErrorMower eErrorMower;
    
    STATUS_updateLedStatus(&eEtatMower, dataTWI);
    STATUS_updateLedError(&eErrorMower, dataTWI);
    STATUS_updateLed(&eEtatMower, &eErrorMower);
}

void STATUS_updateLedStatus(EtatMower* eEtatMower, uint8_t uLedStatus) {
    
    *eEtatMower = uLedStatus & 0x0F;
}

void STATUS_updateLedError(ErrorMower* eErrorMower, uint8_t uLedStatus) {
    
    *eErrorMower = uLedStatus & 0xF0;
}

void STATUS_updateLed(EtatMower* eEtatMower, ErrorMower* eErrorMower) {
    // LED VERT: PD2, ORANGE: PD3, ROUGE: PD4
    switch(*eEtatMower) {
        case UNKNOWN_ETAT:
            PORTD &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            break;
            
        case TACHE_EN_COURS:
            PORTD &= ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            PORTD |= (1<<LED_GREEN);
            break;
            
        case RETOUR_STATION:
            PORTD &= ~(1<<LED_GREEN) & ~(1<<LED_RED);
            PORTD |= (1<<LED_ORANGE);
            break;
            
        case EN_CHARGE:
            PORTD &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            _delay_ms(500);
            PORTD |= (1<<LED_GREEN) | (1<<LED_ORANGE) | (1<<LED_RED);
            _delay_ms(500);
            break;
            
        case PAS_DE_TACHE_EN_COURS:
            PORTD &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE);
            PORTD |= (1<<LED_RED);
            break;
            
        case PAUSE:
            PORTD &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            _delay_ms(500);
            PORTD |= (1<<LED_GREEN);
            _delay_ms(500);
            break;
            
        default:
            PORTD &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            break;
    }
    
    // LED JAUNE1: PD5, JAUNE2: PD6, JAUNE3: PD7
    switch(*eErrorMower) {
        case NTR:
            PORTD &= ~(1<<LED_YELLOW_1) & ~(1<<LED_YELLOW_2) & ~(1<<LED_YELLOW_3);
            break;
            
        case BLOCKED_MOWER:
            PORTD &= ~(1<<LED_YELLOW_1) & ~(1<<LED_YELLOW_2);
            PORTD |= (1<<LED_YELLOW_3);
            break;
            
        case DETECTED_RAIN:
            PORTD &= ~(1<<LED_YELLOW_1) & ~(1<<LED_YELLOW_3);
            PORTD |= (1<<LED_YELLOW_2);
            break;
            
        case WIRE_NOT_DETECTED:
            PORTD &= ~(1<<LED_YELLOW_1);
            PORTD |= (1<<LED_YELLOW_2) | (1<<LED_YELLOW_3);
            break;
            
        case LOW_BATTERY:
            PORTD &= ~(1<<LED_YELLOW_3) & ~(1<<LED_YELLOW_2);
            PORTD |= (1<<LED_YELLOW_1);
            break;
            
        case VERY_LOW_BATTERY:
            PORTD &= ~(1<<LED_YELLOW_2);
            PORTD |= (1<<LED_YELLOW_1) | (1<<LED_YELLOW_3);
            break;
            
        case EMPTY_BATTERY:
            PORTD &= ~(1<<LED_YELLOW_3);
            PORTD |= (1<<LED_YELLOW_1) | (1<<LED_YELLOW_2);
            break;
            
        default:
            PORTD &= ~(1<<LED_YELLOW_1) & ~(1<<LED_YELLOW_3) & ~(1<<LED_YELLOW_2);
            break;
    }
}
