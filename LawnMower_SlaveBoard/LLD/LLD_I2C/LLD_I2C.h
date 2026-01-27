/**
 * @file LLD_I2C.h
 * @author MVE
 * @brief Header file for I2C peripheral
 * @details
**/

#ifndef LLD_I2C_H_
#define LLD_I2C_H_
/*--------------------------------------------------------------------------*/
/* ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
#define F_CPU 20000000UL
#define SCL_CLOCK  400000UL
/*** Adresses I2C ***/
#define ADDR_SLAVE_SENSOR 0x20

typedef enum 
{
	E_SLAVE_READ_DATA_V,
	E_SLAVE_READ_DATA_A,
	E_SLAVE_READ_DATA_DOCK,
    E_SLAVE_READ_DATA_TIME_TO_MOW,
	E_SLAVE_READ_DATA_SONAR_FC,
	E_SLAVE_READ_DATA_SONAR_FL,
	E_SLAVE_READ_DATA_SONAR_FR,
	E_SLAVE_READ_DATA_GPS_LONG_MMSB,
	E_SLAVE_READ_DATA_GPS_LONG_MSB,
	E_SLAVE_READ_DATA_GPS_LONG_LSB,
	E_SLAVE_READ_DATA_GPS_LONG_LLSB,
	E_SLAVE_READ_DATA_GPS_LAT_MMSB,
	E_SLAVE_READ_DATA_GPS_LAT_MSB,
	E_SLAVE_READ_DATA_GPS_LAT_LSB,
	E_SLAVE_READ_DATA_GPS_LAT_LLSB,
    E_SLAVE_READ_DATA_LED_STATUS,
	
	E_SLAVE_READ_DATA_NUMBER
}E_SLAVE_READ_DATA;

typedef struct
{
    uint8_t u8_dataV;
    uint8_t u8_dataA;
    uint8_t u8_dataDock;
    uint8_t u8_dataTimeToMow;

    uint8_t u8_dataSonarFc;
    uint8_t u8_dataSonarFl;
    uint8_t u8_dataSonarFr;

    uint8_t u8_dataGpsLongMMSB;
    uint8_t u8_dataGpsLongMSB;
    uint8_t u8_dataGpsLongLSB;
    uint8_t u8_dataGpsLongLLSB;
    
    uint8_t u8_dataGpsLatMMSB;
    uint8_t u8_dataGpsLatMSB;
    uint8_t u8_dataGpsLatLSB;
    uint8_t u8_dataGpsLatLLSB;

    uint8_t u8_dataLedStatus;
}ST_DATA_SLAVE_SENSOR;

/*--------------------------------------------------------------------------*/
/*! ... GLOBAL FUNCTIONS DECLARATIONS ...                                   */
/*--------------------------------------------------------------------------*/
void LLD_I2C_Init(void);
uint8_t LLD_I2C_IsInterruptRunning(void);
ST_DATA_SLAVE_SENSOR* LLD_I2C_GetStructPointer(void);
uint8_t LLD_I2C_Read(uint8_t u8_slaveAddr, uint8_t u8_dataAddr, uint8_t *pu8_receivedData);
uint8_t LLD_I2C_Write(uint8_t u8_slaveAddr, uint8_t u8_dataAddr, uint8_t u8_data);
/*--------------------------------------------------------------------------*/
/* ... END OF FILE...                                                      */
/*--------------------------------------------------------------------------*/
#endif /* LLD_I2C_H_ */
