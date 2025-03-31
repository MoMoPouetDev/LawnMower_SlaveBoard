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

/**********************************************/
//	0%   | 9     | 1,8   | 368,0981595
//	5%   | 9,9   | 1,98  | 404,9079755
//	10%  | 10,8  | 2,16  | 441,7177914
//	15%  | 10,95 |  2,19 | 447,8527607
//	20%  | 11,1  | 2,22  | 453,9877301
//	25%  | 11,18 | 2,236 | 457,2597137
//	30%  | 11,25 | 2,25  | 460,1226994
//	40%  | 11,37 | 2,274 | 465,0306748
//	50%  | 11,49 | 2,298 | 469,9386503
//	60%  | 11,61 | 2,322 | 474,8466258
//	70%  | 11,76 | 2,352 | 480,9815951
//	75%  | 11,84 | 2,368 | 484,2535787
//	80%  | 11,91 | 2,382 | 487,1165644
//	85%  | 12,1  | 2,42  | 494,8875256
//	90%  | 12,3  | 2,46  | 503,0674847
//	95%  | 12,45 | 2,49  | 509,202454
//	100% | 12,6  | 2,52  | 515,3374233
/*********************************************/
uint8_t SENSORS_getBatteryPercent() {
	uint16_t uTension = ADC_read(PIN_ADC0);
	uint8_t uPurcentage;
	
	if(uTension < 370) { uPourcentage = 0; }
	else if(uTension >= 370 && uTension < 405) { uPourcentage = 5; }
	else if(uTension >= 405 && uTension < 442) { uPourcentage = 10; }
	else if(uTension >= 442 && uTension < 448) { uPourcentage = 15; }
	else if(uTension >= 448 && uTension < 454) { uPourcentage = 20; }
	else if(uTension >= 454 && uTension < 457) { uPourcentage = 25; }
	else if(uTension >= 457 && uTension < 460) { uPourcentage = 30; }
	else if(uTension >= 460 && uTension < 465) { uPourcentage = 40; }
	else if(uTension >= 465 && uTension < 470) { uPourcentage = 50; }
	else if(uTension >= 470 && uTension < 475) { uPourcentage = 60; }
	else if(uTension >= 475 && uTension < 481) { uPourcentage = 70; }
	else if(uTension >= 481 && uTension < 484) { uPourcentage = 75; }
	else if(uTension >= 484 && uTension < 487) { uPourcentage = 80; }
	else if(uTension >= 487 && uTension < 495) { uPourcentage = 85; }
	else if(uTension >= 495 && uTension < 503) { uPourcentage = 90; }
	else if(uTension >= 503 && uTension < 509) { uPourcentage = 95; }
	else if(uTension >= 509) { uPourcentage = 100; }
	
	return uPourcentage;
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
        return (uint8_t)SONAR_DIST_ERR;
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
