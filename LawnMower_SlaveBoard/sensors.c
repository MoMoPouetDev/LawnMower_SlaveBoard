//
//  sensors.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include <stdio.h>
#include <util/delay.h>

#include "constant.h"
#include "sensors.h"
#include "adc.h"
#include "gps.h"
#include "status.h"

uint8_t SENSORS_isTimeToMow()
{
	if ((THRESHOLD_HOUR_MIN <= _uHoursGpsAcquisition) && (_uHoursGpsAcquisition < THRESHOLD_HOUR_MAX))
		return 1;
	else
		return 0;
}

uint8_t SENSORS_isCharging()
{
	if (ADC_read(PIN_ADC1) <= CHARGING_THRESHOLD)
	    return 0;
    else
	    return 1;
}

uint8_t SENSORS_isRaining()
{
    if (ADC_read(PIN_ADC2) <= RAINING_THRESHOLD)
		return 0;
    else
		return 1;
}

uint8_t SENSORS_getBatteryPercent() {
	uint16_t uTension = ADC_read(PIN_ADC0);
	// 0 -> 1023
	// 0% -> 100%
	return ((uTension*100)/1023);
}

uint8_t SENSORS_getSonarDistance(uint8_t sonarID)
{
    uint32_t distance = SONAR_DIST_ERR;
    uint32_t tempCount = 0;
    uint8_t echoID;
    
    if(sonarID == PIN_TRIG_FC)
        echoID = PIN_ECHO_FC;
    else if(sonarID == PIN_TRIG_FL)
        echoID = PIN_ECHO_FL;
    else if(sonarID == PIN_TRIG_FR)
        echoID = PIN_ECHO_FR;
    else
    {
        echoID = PINB0;
        return SONAR_DIST_ERR;
    }
    
    _uTimerOvfCount = 0;
    
    PORTB |= (1<<sonarID);
    _delay_us(12);
    PORTB &= ~(1<<sonarID);
    
    while(!(PINB & (1<<echoID)));
    TCNT1 = 0;
    while(PINB & (1<<echoID));
    tempCount = TCNT1 + (TIMER1_OVERFLOW*_uTimerOvfCount);
    
    distance = (tempCount / TIMER_DISTANCE)/2;
    
    if(distance > THRESHOLD_8_BITS) {
	    distance = THRESHOLD_8_BITS;
	}
	
    return (uint8_t)distance;
}

void myDelayLoop(double delay)
{
    double i;
    for (i=0; i<delay; i++) {
        _delay_ms(1);
    }
}
