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
        case ADDR_SENSOR_V:
            _uSendData = _uBatteryPercent;
            break;
            
        case ADDR_SENSOR_A:
            _uSendData = _uChargeLevel;
            break;
            
        case ADDR_SENSOR_DOCK:
            _uSendData = _eEtatDock;
            break;
            
        case ADDR_SENSOR_RAIN:
            _uSendData = _uUnderTheRain;
            break;
            
        case ADDR_SONAR_FC:
            _uSendData = _uDistanceSonarFC;
            break;
            
        case ADDR_SONAR_FL:
            _uSendData = _uDistanceSonarFL;
            break;
            
        case ADDR_SONAR_FR:
            _uSendData = _uDistanceSonarFR;
            break;
			
        case ADDR_GPS_TIME_HOURS:
            _uSendData = _uHoursGpsAcquisition;
            break;
			
		case ADDR_GPS_TIME_MINUTES:
            _uSendData = _uMinutesGpsAcquisition;
            break;
			
        case ADDR_GPS_DATE_DAYS:
            _uSendData = _uDaysGpsAcquisition;
            break;
			
		case ADDR_GPS_DATE_MONTHS:
            _uSendData = _uMonthsGpsAcquisition;
            break;
			
		case ADDR_GPS_LONG_DEG:
            _uSendData = _tLongitude.degrees;
            break;
			
		case ADDR_GPS_LONG_MIN:
            _uSendData = _tLongitude.minutes;
            break;
			
		case ADDR_GPS_LONG_DEC_MSB:
            _uSendData = _tLongitude.decimalMSB;
            break;
        
        case ADDR_GPS_LONG_DEC_B:
            _uSendData = _tLongitude.decimalB;
            break;
        
        case ADDR_GPS_LONG_DEC_LSB:
            _uSendData = _tLongitude.decimalLSB;
            break;
            
        case ADDR_GPS_LAT_DEG:
            _uSendData = _tLatitude.degrees;
            break;
			
        case ADDR_GPS_LAT_MIN:
            _uSendData = _tLatitude.minutes;
            break;
			
        case ADDR_GPS_LAT_DEC_MSB:
            _uSendData = _tLatitude.decimalMSB;
            break;
            
        case ADDR_GPS_LAT_DEC_B:
            _uSendData = _tLatitude.decimalB;
            break;
            
        case ADDR_GPS_LAT_DEC_LSB:
            _uSendData = _tLatitude.decimalLSB;
            break;
			
		case ADDR_TIME_TO_MOW:
			_uSendData = isTimeToMow();
            
        default:
            _uSendData = ADDR_UNKNOWN_DATA;
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
