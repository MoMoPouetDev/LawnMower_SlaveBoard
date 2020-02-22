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


#include "constant.h"

ISR(TWI_vect)
{
    
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
