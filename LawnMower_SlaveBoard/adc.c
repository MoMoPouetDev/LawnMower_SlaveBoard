//
//  adc.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include <avr/io.h>

#include "adc.h"

uint16_t ADC_read(uint8_t adcChannel)
{
    adcChannel &= 0x07;
    ADMUX = ( ADMUX & 0xF8 ) | adcChannel; // Mask pour selection de l'adc
    
    ADCSRA |= (1<<ADSC); // Start Conversion
    
    while (ADCSRA & (1<<ADSC)); // Wait fin conversion
    
    return ADC;
}