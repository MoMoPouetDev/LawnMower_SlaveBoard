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

void Initialisation()
{
    INIT_io();
    INIT_variable();
    INIT_i2c();
    INIT_uart();
    INIT_adc();
    INIT_timer();
    INIT_interrupt();
}

void INIT_io()
{
    /***** PORT B *****/
    DDRB = 0x00;
    //DDRB |= (0<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3); // Echo Sonar FC, FL, FR, BC
    DDRB |= (1<<DDB4) | (1<<DDB5) | (1<<DDB6) | (1<<DDB7); // Trigger Sonar FC, FL, FR, BC
    
    PORTB = 0x00;
    //PORTB |= (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB3); // No Pull-Up Echo Sonar
    // PORTB &= ~(1<<PORTB4) & ~(1<<PORTB5) & ~(1<<PORTB6) & ~(1<<PORTB7); // Force à 0 Trigger
    
    
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
    //DDRD |= (1<<DDD3); // Entrée Docking
    //DDRD |= (1<<DDD2) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7); // TBD
    
    PORTD = 0x00;
    //PORTD |= (1<<PORTD0) | (1<<PORTD1); // UART - RX No Pull-Up - TX
    PORTD |= (1<<PORTD3); // Pull up Docking
    PORTD |= (1<<PORTD2) | (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7); // TBD - Pull-Up
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

void INIT_i2c()
{
    TWBR = 32; //TWBR  = ((F_CPU / SCL_CLK) – 16) / 2
    TWCR = (1<<TWEN) | (1<<TWIE);
}

void INIT_uart()
{
    /***** UART BaudRate *****/
    UBRR0H = ((BAUD_PRESCALE)>>8);
    UBRR0L = BAUD_PRESCALE;
    
    /***** Autoriser Reception *****/
    UCSR0B |= (1<<RXEN0);
    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
    
}

void INIT_adc()
{
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1); // Enable ADC et 64 prescale
}

void INIT_variable()
{
	distanceSonarFC = 0;
	distanceSonarFL = 0;
	distanceSonarFR = 0;
	distanceSonarRC = 0;
	timerOvfCount = 0;
	batteryLevel = 0;
	chargeLevel = 0;
	underTheRain = 0;
	_bGpsAcquisition = 0;
	_uHoursGpsAcquisition = 0;
	_uOvfFlag = 0;
}
