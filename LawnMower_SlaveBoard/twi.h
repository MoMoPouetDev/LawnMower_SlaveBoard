//
//  twi.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 21/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef twi_h
#define twi_h

typedef enum {
    UNKNOWN_DATA = 0x00,
    SENSOR_V = 0x01,
    SENSOR_A = 0x02,
    SENSOR_DOCK = 0x03,
    SENSOR_RAIN = 0x04,
    SONAR_FC = 0x05,
    SONAR_FL = 0x06,
    SONAR_FR = 0x07,
    GPS = 0x08
}DataAddress;
DataAddress _eDataAdress;

void TWI_decodeReceivedData(uint8_t);
void TWI_start(void);
void TWI_write(uint8_t, uint8_t);
uint8_t TWI_readACK(void);
uint8_t TWI_readNACK(void);
void TWI_stop(void);

#endif /* twi_h */
