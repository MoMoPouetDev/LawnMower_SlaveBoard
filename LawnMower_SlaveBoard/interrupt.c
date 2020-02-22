//
//  interrupt.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "constant.h"
#include "twi.h"

ISR(TWI_vect)
{
    //uint8_t status = (TWSR & 0xF8);
    uint8_t receivedData = 0;
    
    switch (TW_STATUS) {
        case TW_SR_SLA_ACK:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
            break;
            
        case TW_SR_DATA_ACK:
            receivedData = TWDR;
            TWI_decodeReceivedData(receivedData);
            TWCR = (1<<TWEN) | (1<<TWINT);
            break;
            
        case TW_ST_SLA_ACK:
            TWDR = _uSendData;
            TWCR = (1<<TWEN) | (1<<TWINT);
            break;
            
        case TW_ST_DATA_NACK:
            TWCR = (1<<TWEN) | (1<<TWINT);
            break;
            
        default:
            break;
    }
}

ISR(PCINT2_vect)
{
    if(!(PIND & (1<<PIND3)))
        _eEtatDock = ON;
    else if((PIND & (1<<PIND3)))
        _eEtatDock = OFF;
}

ISR(TIMER1_OVF_vect)
{
    _uTimerOvfCount++;
	if(_uTimerOvfCount == 0xFFFF)
		_uOvfFlag = 1;
}
