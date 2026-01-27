/*
 * RUN_I2C.c
 *
 *  Created on: 12 FEB 2023
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "RUN_I2C.h"
#include "RUN_GPS.h"
#include "RUN_Sensors.h"
#include "RUN_GPIO.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
ST_DATA_SLAVE_SENSOR* gpst_dataSlaveSensor;
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/

void RUN_I2C_Init(void)
{
	HAL_I2C_Init();
    gpst_dataSlaveSensor = LLD_I2C_GetStructPointer();
}

uint8_t RUN_I2C_IsI2cBusy(void)
{
    uint8_t u8_status = 0;
    u8_status = HAL_I2C_IsI2cBusy();
    return u8_status;
}

uint8_t* RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA e_slaveReadData)
{
    uint8_t* pu8_data;

    switch (e_slaveReadData) 
	{
        case E_SLAVE_READ_DATA_V:
			pu8_data = &gpst_dataSlaveSensor->u8_dataV;
            break;
            
        case E_SLAVE_READ_DATA_A:
			pu8_data = &gpst_dataSlaveSensor->u8_dataA;
            break;
            
        case E_SLAVE_READ_DATA_DOCK:
			pu8_data = &gpst_dataSlaveSensor->u8_dataDock;
            break;

		case E_SLAVE_READ_DATA_TIME_TO_MOW:
            pu8_data = &gpst_dataSlaveSensor->u8_dataTimeToMow;
			break;
            
        case E_SLAVE_READ_DATA_SONAR_FC:
			pu8_data = &gpst_dataSlaveSensor->u8_dataSonarFc;
            break;
            
        case E_SLAVE_READ_DATA_SONAR_FL:
			pu8_data = &gpst_dataSlaveSensor->u8_dataSonarFl;
            break;
            
        case E_SLAVE_READ_DATA_SONAR_FR:
			pu8_data = &gpst_dataSlaveSensor->u8_dataSonarFr;
            break;
			
		case E_SLAVE_READ_DATA_GPS_LONG_MMSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLongMMSB;
            break;
			
        case E_SLAVE_READ_DATA_GPS_LONG_MSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLongMSB;
            break;
			
		case E_SLAVE_READ_DATA_GPS_LONG_LSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLongLSB;
            break;
			
		case E_SLAVE_READ_DATA_GPS_LONG_LLSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLongLLSB;
            break;
			
		case E_SLAVE_READ_DATA_GPS_LAT_MMSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLatMMSB;
            break;
        
        case E_SLAVE_READ_DATA_GPS_LAT_MSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLatMSB;
            break;
        
        case E_SLAVE_READ_DATA_GPS_LAT_LSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLatLSB;
            break;
            
        case E_SLAVE_READ_DATA_GPS_LAT_LLSB:
            pu8_data = &gpst_dataSlaveSensor->u8_dataGpsLatLLSB;
            break;
            
        default:
            *pu8_data = 0xFF;
            break;
    }
    return pu8_data;
}
