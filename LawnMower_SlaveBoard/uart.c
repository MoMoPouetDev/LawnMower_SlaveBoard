//
//  uart.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>

#include "uart.h"

unsigned char UART_reception()
{
    /***** Attente de reception *****/
    while (!(UCSR0A & (1<<RXC0)));
    /***** Return received data *****/
    return UDR0;
}