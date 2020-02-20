//
//  constant.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef constant_h
#define constant_h

#include <stdio.h>
#include <avr/io.h>

#define F_CPU 8000000UL

#define PIN_ADC0 0
#define PIN_ADC1 1
#define PIN_ADC2 2

#define ADDR_SLAVE_SENSOR 0x01

#define I2C_WRITE   0
#define I2C_READ    1

#define BAUD 9600
#define BAUD_PRESCALE (F_CPU/ (16UL*BAUD))-1
#define BUFFER_SIZE 80

#define PIN_TRIG_FC PORTB4
#define PIN_TRIG_FL PORTB5
#define PIN_TRIG_FR PORTB6
#define PIN_TRIG_RC PORTB7

#define SONAR_DIST_ERR (uint8_t)999
#define TIMER1_OVERFLOW 65535
/*** Calcul of value timer 343 m/s -> 34300 cm/s
 dist = (speedSound*TIMER)/2 = (34300*TIMER)/2 = 17150*TIMER = 17150 * (TIMER_VALUE * 0.125 * 10^-6)
 ***/
#define TIMER_DISTANCE 466.47

#define CHARGING_THRESHOLD 100
#define RAINING_THRESHOLD 10
#define THRESHOLD_HOUR_MIN 7
#define THRESHOLD_HOUR_MAX 18

uint8_t distanceSonarFC;
uint8_t distanceSonarFL;
uint8_t distanceSonarFR;
uint8_t distanceSonarRC;
uint16_t timerOvfCount;
uint8_t _uOvfFlag;

uint16_t batteryLevel;
uint16_t chargeLevel;
uint16_t underTheRain;

uint8_t _bGpsAcquisition;
uint8_t _uHoursGpsAcquisition;

typedef enum
{
    ON,
    OFF
}Etat;
Etat etatDock;

#endif /* constant_h */