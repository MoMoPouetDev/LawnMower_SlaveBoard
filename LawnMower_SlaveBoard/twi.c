//
//  twi.c
//  LawnMower_SlaveBoard
//
//  Created by morgan on 21/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>
#include <util/twi.h>

#include "constant.h"
#include "twi.h"
#include "sensors.h"
#include "gps.h"

uint8_t TWI_decodeReceivedData(uint8_t receivedData) {
    uint8_t uSendData;
    switch (receivedData) {
        case ADDR_SENSOR_V:
            uSendData = _uBatteryPercent;
            break;
            
        case ADDR_SENSOR_A:
            uSendData = _uChargeLevel;
            break;
            
        case ADDR_SENSOR_DOCK:
            uSendData = _eEtatDock;
            break;
            
        case ADDR_SENSOR_RAIN:
            uSendData = _uUnderTheRain;
            break;
            
        case ADDR_SONAR_FC:
            uSendData = _uDistanceSonarFC;
            break;
            
        case ADDR_SONAR_FL:
            uSendData = _uDistanceSonarFL;
            break;
            
        case ADDR_SONAR_FR:
            uSendData = _uDistanceSonarFR;
            break;
			
        case ADDR_GPS_TIME_HOURS:
            uSendData = _uHoursGpsAcquisition;
            break;
			
		case ADDR_GPS_TIME_MINUTES:
            uSendData = _uMinutesGpsAcquisition;
            break;
			
        case ADDR_GPS_DATE_DAYS:
            uSendData = _uDaysGpsAcquisition;
            break;
			
		case ADDR_GPS_DATE_MONTHS:
            uSendData = _uMonthsGpsAcquisition;
            break;
			
		case ADDR_GPS_LONG_DEG:
            uSendData = _tLongitude.degrees;
            break;
			
		case ADDR_GPS_LONG_MIN:
            uSendData = _tLongitude.minutes;
            break;
			
		case ADDR_GPS_LONG_DEC_MSB:
            uSendData = _tLongitude.decimalMSB;
            break;
        
        case ADDR_GPS_LONG_DEC_B:
            uSendData = _tLongitude.decimalB;
            break;
        
        case ADDR_GPS_LONG_DEC_LSB:
            uSendData = _tLongitude.decimalLSB;
            break;
            
        case ADDR_GPS_LAT_DEG:
            uSendData = _tLatitude.degrees;
            break;
			
        case ADDR_GPS_LAT_MIN:
            uSendData = _tLatitude.minutes;
            break;
			
        case ADDR_GPS_LAT_DEC_MSB:
            uSendData = _tLatitude.decimalMSB;
            break;
            
        case ADDR_GPS_LAT_DEC_B:
            uSendData = _tLatitude.decimalB;
            break;
            
        case ADDR_GPS_LAT_DEC_LSB:
            uSendData = _tLatitude.decimalLSB;
            break;
			
		case ADDR_TIME_TO_MOW:
			uSendData = isTimeToMow();
            
        default:
            uSendData = ADDR_UNKNOWN_DATA;
            break;
    }
    return uSendData;
}

void TWI_start()
{
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
    while (!(TWCR & (1<<TWINT)));
}

void TWI_write(uint8_t addrSlave, uint8_t i2c_read_write)
{
    TWDR = addrSlave + i2c_read_write;
    TWCR = (1<<TWEN) | (1<<TWINT);
    while (!(TWCR & (1<<TWINT)));
}

void TWI_write_data(uint8_t dataToSend)
{
    TWDR = dataToSend;
    TWCR = (1<<TWEN) | (1<<TWINT);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t TWI_readACK()
{
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWI_readNACK()
{
    TWCR = (1<<TWEN) | (1<<TWINT);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void TWI_stop()
{
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}
