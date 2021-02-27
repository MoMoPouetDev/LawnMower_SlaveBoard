//
//  initialisation.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "constant.h"
#include "initialisation.h"
#include "gps.h"

void Initialisation()
{
    INIT_io();
    INIT_variable();
    INIT_twi();
    INIT_uart();
    INIT_adc();
    INIT_timer();
    INIT_interrupt();
}

void INIT_io()
{
    /***** PORT B *****/
    DDRB = 0x00;
    //DDRB |= (0<<DDB0) | (1<<DDB1) | (1<<DDB2) ; // Echo Sonar FC, FL, FR
    DDRB |= (1<<DDB4) | (1<<DDB5) | (1<<DDB3); // Trigger Sonar FC, FL, FR
	//DDRB |= (1<<DDB6); // TBD
    //DDRB |= (1<<DDB7); // TBD
    
    PORTB = 0x00;
    //PORTB |= (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2); // No Pull-Up Echo Sonar
    // PORTB &= ~(1<<PORTB4) & ~(1<<PORTB5) & ~(1<<PORTB3); // Force à 0 Trigger
    PORTB |= (1<<PORTB6) | (1<<PORTB7); // TBD - Pull-Up
    
    /***** PORT C *****/
    DDRC = 0x00;
    //DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC2); // ADC - Capteur Tension, courant, pluie
    //DDRC |= (1<<DDC3); // TBD
    DDRC |= (1<<DDC4) | (1<<DDC5); // Config I2C SDA - SCL
    //DDRC |= (1<<DDC6); // TBD
    
    PORTC = 0x00;
    //PORTC &= ~(1<<PORTC0) & ~(1<<PORTC1) & ~(1<<PORTC2); // ADC - No Pull-Up
    PORTC |= (1<<PORTC3); // TBD - Pull-Up
    //PORTC &= ~(1<<PORTC4) & ~(1<<PORTC5); // I2C - Force à 0
    PORTC |= (1<<PORTC6); // TBD - Pull-Up
    
    /***** PORT D *****/
    DDRD = 0x00;
    DDRD |= (1<<DDD1); //| (0<<DDD0); // UART - TXD - RXD
    DDRD |= (1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7); // LED GREEN - ORANGE - RED - YELLOW1 - 2 - 3
    
    PORTD = 0x00;
    //PORTD |= (1<<PORTD0) | (1<<PORTD1); // UART - RX No Pull-Up - TX
    //PORTD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7); // Force LED 0
}

void INIT_interrupt()
{
    PCICR |= (1<<PCIE2); // Activation des Interruptions sur PCINT[23:16]
    PCMSK2 |= (1<<PCINT19); // Active interruptons sur PCINT19
    sei();
}

void INIT_timer()
{
    //CS1[2:0] CS10 HIGH to launch with no prescal CS10 LOW to stop counter in TCCR1B
    TCCR1B |= (1<<CS10);
    TIMSK1 |= (1<<TOIE1); // Enable overflow counter
}

void INIT_twi()
{
    TWBR  = (uint8_t)(( F_CPU  / SCL_CLOCK ) - 16 ) / 2; //- 400kHz
    TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
	TWAR = ADDR_SLAVE_SENSOR;
}

void INIT_uart()
{
    /***** UART BaudRate *****/
    UBRR0H = (unsigned char) ((BAUD_PRESCALE)>>8);
    UBRR0L = (unsigned char) BAUD_PRESCALE;
    
    /***** Autoriser Reception *****/
    UCSR0B = (1<<RXEN0);
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    
}

void INIT_adc()
{
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1); // Enable ADC et 64 prescale
}

void INIT_variable()
{
	_uDistanceSonarFC = 0;
	_uDistanceSonarFL = 0;
	_uDistanceSonarFR = 0;
	_uTimerOvfCount = 0;
	_uBatteryPercent = 0;
	_uChargeLevel = 0;
	_uUnderTheRain = 0;
	_uOvfFlag = 0;
	_uMinutesGpsAcquisition = 0;
	_uHoursGpsAcquisition = 0;
	_uMonthsGpsAcquisition = 0;
	_uDaysGpsAcquisition = 0;
    _tLatitude.degrees = 0;
    _tLatitude.minutes = 0;
    _tLatitude.decimalMSB = 0;
    _tLatitude.decimalB = 0;
    _tLatitude.decimalLSB = 0;
    _tLongitude.degrees = 0;
    _tLongitude.minutes = 0;
    _tLongitude.decimalMSB = 0;
    _tLongitude.decimalB = 0;
    _tLongitude.decimalLSB = 0;
}
