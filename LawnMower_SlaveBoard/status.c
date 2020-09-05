//
//  status.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 05/09/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include "status.h"
#include <util/delay.h>

void STATUS_updateStatus() {
    EtatMower eEtatMower;
    ErrorMower eErrorMower;
    
    STATUS_updateLedStatus(&eEtatMower);
    STATUS_updateLedError(&eErrorMower);
    STATUS_updateLed(&eEtatMower, &eErrorMower);
}

void STATUS_updateLedStatus(EtatMower* eEtatMower) {
    
    *eEtatMower = _uLedStatus & 0x0F;
}

void STATUS_updateLedError(ErrorMower* eErrorMower) {
    
    *eErrorMower = _uLedStatus & 0xF0;
}

void STATUS_updateLed(EtatMower* eEtatMower, ErrorMower* eErrorMower) {
    // LED VERT: PB4, ORANGE: PB2, ROUGE: PB1
    switch(*eEtatMower) {
        case UNKNOWN_ETAT:
            PORTB &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            break;
            
        case TACHE_EN_COURS:
            PORTB &= ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            PORTB |= (1<<LED_GREEN);
            break;
            
        case RETOUR_STATION:
            PORTB &= ~(1<<LED_GREEN) & ~(1<<LED_RED);
            PORTB |= (1<<LED_ORANGE);
            break;
            
        case EN_CHARGE:
            PORTB &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            _delay_ms(500);
            PORTB |= (1<<PORT4) | (1<<LED_ORANGE) | (1<<LED_RED);
            _delay_ms(500);
            break;
            
        case PAS_DE_TACHE_EN_COURS:
            PORTB &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE);
            PORTB |= (1<<LED_RED);
            break;
            
        case PAUSE:
            PORTB &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            _delay_ms(500);
            PORTB |= (1<<LED_GREEN);
            _delay_ms(500);
            break;
            
        default:
            PORTB &= ~(1<<LED_GREEN) & ~(1<<LED_ORANGE) & ~(1<<LED_RED);
            break;
    }
    
    // LED JAUNE1: PB5, JAUNE2: PC2, JAUNE3: PC3
    switch(*eErrorMower) {
        case NTR:
            PORTB &= ~(1<<LED_YELLOW_1);
            PORTC &= ~(1<<LED_YELLOW_3) & ~(1<<LED_YELLOW_2);
            break;
            
        case BLOCKED_MOWER:
            PORTB &= ~(1<<LED_YELLOW_1);
            PORTC &= ~(1<<LED_YELLOW_2);
            PORTC |= (1<<LED_YELLOW_3);
            break;
            
        case DETECTED_RAIN:
            PORTB &= ~(1<<LED_YELLOW_1);
            PORTC &= ~(1<<LED_YELLOW_3);
            PORTC |= (1<<LED_YELLOW_2);
            break;
            
        case WIRE_NOT_DETECTED:
            PORTB &= ~(1<<LED_YELLOW_1);
            PORTC |= (1<<LED_YELLOW_2) | (1<<LED_YELLOW_3);
            break;
            
        case LOW_BATTERY:
            PORTC &= ~(1<<LED_YELLOW_3) & ~(1<<LED_YELLOW_2);
            PORTB |= (1<<LED_YELLOW_1);
            break;
            
        case VERY_LOW_BATTERY:
            PORTC &= ~(1<<LED_YELLOW_2);
            PORTB |= (1<<LED_YELLOW_1);
            PORTC |= (1<<LED_YELLOW_3);
            break;
            
        case EMPTY_BATTERY:
            PORTC &= ~(1<<LED_YELLOW_3);
            PORTB |= (1<<LED_YELLOW_1);
            PORTC |= (1<<LED_YELLOW_2);
            break;
            
        default:
            PORTB &= ~(1<<LED_YELLOW_1);
            PORTC &= ~(1<<LED_YELLOW_3) & ~(1<<LED_YELLOW_2);
            break;
    }
}
