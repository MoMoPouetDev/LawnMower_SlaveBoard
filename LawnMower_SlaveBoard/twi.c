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

void TWI_decodeReceivedData(uint8_t receivedData) {
    switch (receivedData) {
        case SENSOR_V:
            _uSendData = _uBatteryPercent;
            break;
            
        case SENSOR_A:
            _uSendData = _uChargeLevel;
            break;
            
        case SENSOR_DOCK:
            _uSendData = _eEtatDock;
            break;
            
        case SENSOR_RAIN:
            _uSendData = _uUnderTheRain;
            break;
            
        case SONAR_FC:
            _uSendData = _uDistanceSonarFC;
            break;
            
        case SONAR_FL:
            _uSendData = _uDistanceSonarFL;
            break;
            
        case SONAR_FR:
            _uSendData = _uDistanceSonarFR;
            break;
            
        case SONAR_RC:
            _uSendData = _uDistanceSonarFR;
            break;
			
        case GPS_TIME_HOURS:
            _uSendData = _uHoursGpsAcquisition;
            break;
			
		case GPS_TIME_MINUTES:
            _uSendData = _uMinutesGpsAcquisition;
            break;
			
        case GPS_DATE_DAYS:
            _uSendData = _uDaysGpsAcquisition;
            break;
			
		case GPS_DATE_MONTHS:
            _uSendData = _uMonthsGpsAcquisition;
            break;
			
		case GPS_LONG_DEG:
            _uSendData = _tLongitude.degrees;
            break;
			
		case GPS_LONG_MIN:
            _uSendData = _tLongitude.minutes;
            break;
			
		case GPS_LONG_DEC:
            _uSendData = _tLongitude.decimal;
            break;
			
        case GPS_LAT_DEG:
            _uSendData = _tLatitude.degrees;
            break;
			
        case GPS_LAT_MIN:
            _uSendData = _tLatitude.minutes;
            break;
			
        case GPS_LAT_DEC:
            _uSendData = _tLatitude.decimal;
            break;
			
		case TIME_TO_MOW:
			_uSendData = isTimeToMow();
            
        default:
            _uSendData = UNKNOWN_DATA;
            break;
    }
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
