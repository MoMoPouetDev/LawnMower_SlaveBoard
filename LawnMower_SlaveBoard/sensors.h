//
//  sensors.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef sensors_h
#define sensors_h

uint8_t SENSORS_isDocking(void);
uint8_t SENSORS_isCharging(void);
uint8_t SENSORS_isTimeToMow(void);
uint8_t SENSORS_isRaining(void);
void SENSORS_startSensors(void);
uint8_t SENSORS_getBatteryPercent();
uint8_t SENSORS_getSonarDistance(uint8_t);
void myDelayLoop(double);

#endif /* sensors_h */