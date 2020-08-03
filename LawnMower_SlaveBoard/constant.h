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
#define SCL_CLOCK  400000L

/*** Convertisseur Analogique Numérique ***/
#define PIN_ADC0 0
#define PIN_ADC1 1
#define PIN_ADC2 2
/*** END ***/

/*** Adresses I2C ***/
#define ADDR_SLAVE_SENSOR 0x02

#define ADDR_SENSOR_V 0x01
#define ADDR_SENSOR_A 0x02
#define ADDR_SENSOR_DOCK 0x03
#define ADDR_SENSOR_RAIN 0x04
#define ADDR_SONAR_FC 0x05
#define ADDR_SONAR_FL 0x06
#define ADDR_SONAR_FR 0x07
#define ADDR_GPS_TIME_HOURS 0x08
#define ADDR_GPS_TIME_MINUTES 0x09
#define ADDR_GPS_DATE_DAYS 0x0A
#define ADDR_GPS_DATE_MONTHS 0x0B
#define ADDR_GPS_LONG_DEG 0x0C
#define ADDR_GPS_LONG_MIN 0x0D
#define ADDR_GPS_LONG_DEC_MSB 0x0E
#define ADDR_GPS_LONG_DEC_B 0x0F
#define ADDR_GPS_LONG_DEC_LSB 0x10
#define ADDR_GPS_LAT_DEG 0x11
#define ADDR_GPS_LAT_MIN 0x12
#define ADDR_GPS_LAT_DEC_MSB 0x13
#define ADDR_GPS_LAT_DEC_B 0x14
#define ADDR_GPS_LAT_DEC_LSB 0x15
#define ADDR_TIME_TO_MOW 0x16
#define ADDR_UNKNOWN_DATA 0x00

/*** END ***/

/*** GPS ***/
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU/ (16UL*BAUD))-1)
#define BUFFER_SIZE 80

volatile uint8_t _uOvfFlag;
/*** END ***/

/*** SONAR ***/
#define PIN_TRIG_FC PORTB4
#define PIN_TRIG_FL PORTB5
#define PIN_TRIG_FR PORTB6

#define PIN_ECHO_FC PINB0
#define PIN_ECHO_FL PINB1
#define PIN_ECHO_FR PINB2

#define SONAR_DIST_ERR (uint8_t)999
#define TIMER1_OVERFLOW 65535
/*** Calcul of value timer 343 m/s -> 34300 cm/s
 dist = (speedSound*TIMER)/2 = (34300*TIMER)/2 = 17150*TIMER = 17150 * (TIMER_VALUE * 0.125 * 10^-6)
 ***/
#define TIMER_DISTANCE 466.47

uint8_t _uDistanceSonarFC;
uint8_t _uDistanceSonarFL;
uint8_t _uDistanceSonarFR;

volatile uint16_t _uTimerOvfCount;
/*** END ***/

/*** Capteur Tension ***/
#define CHARGING_THRESHOLD 100

uint8_t _uBatteryPercent;
uint8_t _uChargeLevel;
/*** END ***/

/*** Capteur Pluie ***/
#define RAINING_THRESHOLD 10

uint8_t _uUnderTheRain;
/*** END ***/

/*** Time to Mow ***/
#define THRESHOLD_HOUR_MIN 7
#define THRESHOLD_HOUR_MAX 18
/*** END ***/

#define THRESHOLD_8_BITS 0xFE

typedef enum
{
    ON = 0x01,
    OFF = 0x00
}Etat;
volatile Etat _eEtatDock;

#endif /* constant_h */
