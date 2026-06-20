/*
 * RUN_I2C.c
 *
 *  Created on: 12 FEB 2023
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "HAL_I2C.h"
#include "RUN_I2C.h"
#include "RUN_GPS.h"
#include "RUN_Sensors.h"
#include "RUN_GPIO.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static void _RUN_I2C_Callback(uint8_t receivedData, uint8_t* uSendData);
static uint8_t _RUN_I2C_DecodeReceivedData(uint8_t u8_receivedData);
/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
/**
* @brief		I2C callback
* @param		e_I2c : I2C number
* @return		void
* @details
**/
static void _RUN_I2C_Callback(uint8_t u8_receivedData, uint8_t* pu8_sendData)
{
	static uint8_t _u8_flagLed = 0;
	uint8_t u8_data = 0;

	if (!_u8_flagLed) 
	{
        if (u8_receivedData == E_SLAVE_READ_DATA_LED_STATUS)
        {
            _u8_flagLed = 1;
        }
		else  
		{
			*pu8_sendData = _RUN_I2C_DecodeReceivedData(u8_receivedData);
		}
	}
	else {
		RUN_GPIO_UpdateMowerState(u8_receivedData);
		_u8_flagLed = 0;
	}
}
void RUN_I2C_Init(void)
{
	HAL_I2C_Init(_RUN_I2C_Callback);
}

uint8_t RUN_I2C_IsI2cBusy(void)
{
    uint8_t u8_status = 0;
    u8_status = HAL_I2C_IsI2cBusy();
    return u8_status;
}

static uint8_t _RUN_I2C_DecodeReceivedData(uint8_t u8_receivedData) 
{
    uint8_t u8_sendData;

    switch (u8_receivedData) 
	{
        case E_SLAVE_READ_DATA_V:
			u8_sendData = RUN_Sensors_GetBatteryPercent();
            break;
            
        case E_SLAVE_READ_DATA_A:
			u8_sendData = RUN_Sensors_IsCharging();
            break;
            
        case E_SLAVE_READ_DATA_DOCK:
			u8_sendData = RUN_Sensors_GetDockState();
            break;

		case E_SLAVE_READ_DATA_TIME_TO_MOW:
            u8_sendData = RUN_Sensors_IsTimeToMow();
			break;
            
        case E_SLAVE_READ_DATA_SONAR_FC:
			u8_sendData = RUN_Sensors_SonarGetDistanceFC();
            break;
            
        case E_SLAVE_READ_DATA_SONAR_FL:
			u8_sendData = RUN_Sensors_SonarGetDistanceFL();
            break;
            
        case E_SLAVE_READ_DATA_SONAR_FR:
			u8_sendData = RUN_Sensors_SonarGetDistanceFR();
            break;
			
        case E_SLAVE_READ_DATA_GPS_LONG_DEG:
			u8_sendData = RUN_GPS_GetLongitudeDegrees();
            break;
			
		case E_SLAVE_READ_DATA_GPS_LONG_MIN:
            u8_sendData = RUN_GPS_GetLongitudeMinutes();
            break;
			
        case E_SLAVE_READ_DATA_GPS_LONG_DEC_MSB:
            u8_sendData = RUN_GPS_GetLongitudeDecimalMSB();
            break;
			
		case E_SLAVE_READ_DATA_GPS_LONG_DEC_B:
            u8_sendData = RUN_GPS_GetLongitudeDecimalB();
            break;
			
		case E_SLAVE_READ_DATA_GPS_LONG_DEC_LSB:
            u8_sendData = RUN_GPS_GetLongitudeDecimalLSB();
            break;
			
		case E_SLAVE_READ_DATA_GPS_LAT_DEG:
            u8_sendData = RUN_GPS_GetLatitudeDegrees();
            break;
			
		case E_SLAVE_READ_DATA_GPS_LAT_MIN:
            u8_sendData = RUN_GPS_GetLatitudeMinutes();
            break;
        
        case E_SLAVE_READ_DATA_GPS_LAT_DEC_MSB:
            u8_sendData = RUN_GPS_GetLatitudeDecimalMSB();
            break;
        
        case E_SLAVE_READ_DATA_GPS_LAT_DEC_B:
            u8_sendData = RUN_GPS_GetLatitudeDecimalB();
            break;
            
        case E_SLAVE_READ_DATA_GPS_LAT_DEC_LSB:
            u8_sendData = RUN_GPS_GetLatitudeDecimalLSB();
            break;
            
        default:
            u8_sendData = 0xFF;
            break;
    }
    return u8_sendData;
}
