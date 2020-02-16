//
//  sensors.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef sensors_h
#define sensors_h

uint8_t isDocking(void);
uint8_t isCharging(void);
uint8_t isTimeToMow(void);
uint8_t isRaining(void);
void startSensors(void);
uint8_t getBatteryPercent(uint8_t);
uint8_t getSonarDistance(uint8_t);
void myDelayLoop(double);

#endif /* sensors_h */