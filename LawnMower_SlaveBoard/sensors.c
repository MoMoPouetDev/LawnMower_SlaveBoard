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

uint8_t isDocking()
{
    if(_eEtatDock == ON)
        return 1;
    else
        return 0;
}

uint8_t isCharging()
{
	if(_uChargeLevel >= CHARGING_THRESHOLD)
		return 1; 
    else
		return 0;
}

uint8_t isTimeToMow()
{
	if ((THRESHOLD_HOUR_MIN <= _uHoursGpsAcquisition) && (_uHoursGpsAcquisition < THRESHOLD_HOUR_MAX))
		return 1;
	else
		return 0;
}

uint8_t isRaining()
{
	if(_uUnderTheRain >= RAINING_THRESHOLD)
		return 1; 
    else
		return 0;
}

void startSensors()
{
	uint8_t _uBatteryLevel = 0;
	
    _uBatteryLevel = ADC_read(PIN_ADC0);
	_uBatteryPercent = getBatteryPercent(_uBatteryLevel);
    
    if (ADC_read(PIN_ADC1) <= CHARGING_THRESHOLD)
        _uChargeLevel = 0;
    else
        _uChargeLevel = 1;
    
    if (ADC_read(PIN_ADC2) <= RAINING_THRESHOLD)
        _uUnderTheRain = 0;
    else
        _uUnderTheRain = 1;
    
    _uDistanceSonarFC = getSonarDistance(PIN_TRIG_FC);
    _uDistanceSonarFL = getSonarDistance(PIN_TRIG_FL);
    _uDistanceSonarFR = getSonarDistance(PIN_TRIG_FR);
    _uDistanceSonarRC = getSonarDistance(PIN_TRIG_RC);
	
	_bGpsAcquisition = startGpsAcquisition();
}

uint8_t getBatteryPercent(uint8_t battery) {
	// 0 -> 1023
	// 0% -> 100%
	return ((battery*100)/1023);
}

uint8_t getSonarDistance(uint8_t sonarID)
{
    uint8_t distance = SONAR_DIST_ERR;
    uint32_t tempCount = 0;
    uint8_t echoID;
    
    if(sonarID == PORTB4)
        echoID = PINB0;
    else if(sonarID == PORTB5)
        echoID = PINB1;
    else if(sonarID == PORTB6)
        echoID = PINB2;
    else if(sonarID == PORTB7)
        echoID = PINB3;
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
    
    distance = (uint8_t)((double)tempCount / TIMER_DISTANCE);
    
    return distance;
}

void myDelayLoop(double delay)
{
    double i;
    for (i=0; i<delay; i++) {
        _delay_ms(1);
    }
}
