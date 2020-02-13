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

uint8_t isDocking()
{
    if(etatDock == ON)
        return 1;
    else if (etatDock == OFF)
        return 0;
    else
        return 0
}

uint8_t isCharging()
{
    return chargeLevel;
}

uint8_t isTimeToMow()
{
    return 1;
}

uint8_t isRaining()
{
    return underTheRain;
}

void startSensors()
{
    batteryLevel = ADC_read(PIN_ADC0);
    chargeLevel = ADC_read(PIN_ADC1);
    underTheRain = ADC_read(PIN_ADC2);
    
    distanceSonarFC = getSonarDistance(PIN_TRIG_FC);
    distanceSonarFL = getSonarDistance(PIN_TRIG_FL);
    distanceSonarFR = getSonarDistance(PIN_TRIG_FR);
    distanceSonarRC = getSonarDistance(PIN_TRIG_RC);
}

uint8_t getSonarDistance(uint8_t sonarID)
{
    uint8_t distance = SONAR_DIST_ERR;
    uint32_t tempCount = 0;
    uint8_t echoID;
    
    if(sonarID == PORTB4)
        echoID = PORTB0;
    else if(sonarID == PORTB5)
        echoID = PORTB1;
    else if(sonarID == PORTB6)
        echoID = PORTB2;
    else if(sonarID == PORTB7)
        echoID = PORTB3;
    else
    {
        echoID = PORT0;
        return SONAR_DIST_ERR;
    }
    
    timerOvfCount = 0;
    
    PORTB |= (1<<sonarID);
    _delay_us(12);
    PORTB &= ~(1<<sonarID);
    
    while(!(PORTB & (1<<echoID)));
    TCNT1 = 0;
    while(PORTB & (1<<echoID));
    tempCount = TCNT1 + (TIMER1_OVERFLOW*timerOvfCount);
    
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
