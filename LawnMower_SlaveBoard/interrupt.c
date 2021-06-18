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
#include "status.h"
#include "twi.h"

ISR(TWI_vect)
{
    uint8_t receivedData = 0;
    uint8_t data = 0;
    static uint8_t uSendData;
    static uint8_t flagLed = 0;
    
    switch (TW_STATUS) {
        case TW_SR_SLA_ACK:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA) | (1<<TWIE);
            break;
            
        case TW_SR_DATA_ACK:
            receivedData = TWDR;
            if (!flagLed) {
                if((data = TWI_decodeReceivedData(receivedData)) != ADDR_LED_STATUS) {
                    uSendData = data;
                }
                else if (receivedData == ADDR_LED_STATUS) {
                    flagLed = 1;
                }
            }
            else {
				STATUS_updateStatus(receivedData);
                flagLed = 0;
            }
            
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA)| (1<<TWIE);
            break;
			
		case TW_SR_STOP:
			TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWEN) | (1<<TWINT);
			break;
            
        case TW_ST_SLA_ACK:
            TWDR = uSendData;
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA) | (1<<TWIE);
            break;
            
        case TW_ST_DATA_NACK:
            TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWIE) | (1<<TWEA);
            break;
            
        case TW_BUS_ERROR:
            TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE)| (1<<TWSTO) | (1<<TWINT);
            break;
            
        default:
            TWCR = (1<<TWINT) | (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
            break;
    }
}

ISR(TIMER1_OVF_vect)
{
    _uTimerOvfCount++;
	if(_uTimerOvfCount == 0xFFFF)
		_uOvfFlag = 1;
}

ISR(WDT_vect) {
	_uFlagWatchdog = 1;
	WDTCSR |= (1<<WDIE);
}
