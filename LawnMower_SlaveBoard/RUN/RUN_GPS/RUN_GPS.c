/*
 * RUN_GPS.c
 *
 *  Created on: 25 FEB 2023
 *      Author: morgan.venandy
 */

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HAL_UART.h"
#include "RUN_GPS.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
static U_COORDINATES guf_latitude;
static U_COORDINATES guf_longitude;

static uint8_t gu8_minutesGpsAcquisition;
static uint8_t gu8_hoursGpsAcquisition;
static uint8_t gu8_monthsGpsAcquisition;
static uint8_t gu8_daysGpsAcquisition;
/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static void _RUN_GPS_initBufferNmea(BufferNmea*);
static void _RUN_GPS_initDataRmc(DataNmea_RMC*);
static uint8_t _RUN_GPS_decodeNmeaBuffer(BufferNmea*, DataNmea_RMC*);
static uint8_t _RUN_GPS_decodeNmeaRmc(BufferNmea*, DataNmea_RMC*);
static uint8_t _RUN_GPS_getNmeaUart(BufferNmea*, DataNmea_RMC*);
static uint8_t _RUN_GPS_getNmeaChecksum(char*);
static uint8_t _RUN_GPS_getNmeaBuffer(BufferNmea*, char);
static void _RUN_GPS_decodeNmeaForMaster(DataNmea_RMC*);
static void _RUN_GPS_rmcUtcTime(DataNmea_RMC*);
static void _RUN_GPS_rmcDate(DataNmea_RMC*);
static void _RUN_GPS_rmcLatLong(DataNmea_RMC*);
/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
void RUN_GPS_Init(void) 
{
    gu8_minutesGpsAcquisition = 0;
    gu8_hoursGpsAcquisition = 0;
    gu8_monthsGpsAcquisition = 0;
    gu8_daysGpsAcquisition = 0;

    guf_latitude.f_coordinates = 0.0;

    guf_longitude.f_coordinates = 0.0;
}

void RUN_GPS_startGpsAcquisition(void) 
{
	static uint8_t _u8_gpsState = 0;
	static BufferNmea _pBuffer;
    static DataNmea_RMC _pNmeaRmc;
	uint8_t _bDecodeNmea = 0;
    
	switch (_u8_gpsState)
	{
	case 0:
		_RUN_GPS_initBufferNmea(&_pBuffer);
		_RUN_GPS_initDataRmc(&_pNmeaRmc);
		_u8_gpsState++;
		break;

	case 1:
		_bDecodeNmea = _RUN_GPS_getNmeaUart(&_pBuffer, &_pNmeaRmc);
		if (_bDecodeNmea)
		{
			_u8_gpsState++;
		}
		break;

	case 2:
		_RUN_GPS_decodeNmeaForMaster(&_pNmeaRmc);
		_u8_gpsState = 0;
		break;
		
	default:
		break;
	}
}

static void _RUN_GPS_initBufferNmea(BufferNmea *pBuffer) 
{
    for (int i = 0; i < BUFFER_SIZE; i++) 
	{
        pBuffer->data[i] = 0;
    }
    pBuffer->indice = 0;
    pBuffer->nmea = 0;
}

static void _RUN_GPS_initDataRmc(DataNmea_RMC *pNmeaRmc) 
{
    for (unsigned int i = 0; i<sizeof(pNmeaRmc->utcTime); i++) 
	{
        pNmeaRmc->utcTime[i] = 0;
        pNmeaRmc->utcDate[i] = 0;
    }
    for (unsigned int i = 0; i<sizeof(pNmeaRmc->latitude); i++) 
	{
        pNmeaRmc->latitude[i] = 0;
        pNmeaRmc->longitude[i] = 0;
    }
    for (unsigned int i = 0; i<sizeof(pNmeaRmc->speed); i++) 
	{
        pNmeaRmc->speed[i] = 0;
        pNmeaRmc->cap[i] = 0;
        pNmeaRmc->declMagn[i] = 0;
    }
    pNmeaRmc->latitudeDir = 0;
    pNmeaRmc->longitudeDir = 0;
    pNmeaRmc->declMagnDir = 0;
}

static uint8_t _RUN_GPS_getNmeaUart(BufferNmea *pBuffer, DataNmea_RMC *pNmeaRmc) 
{
	static char _tUartRxBuffer[BUFFER_SIZE] = { 0 };
	static uint8_t _u8_gpsState = 0;
	static uint8_t _cUartRxCounter = 0;
	uint8_t _bTrameNmeaBuffer = 0;
	uint8_t _bDecodeNmeaBuffer = 0;
    uint8_t u8_returnValueUART = 0;
	
	switch (_u8_gpsState)
	{
		case 0:
			u8_returnValueUART = HAL_UART_ReceiveCommand(_tUartRxBuffer, BUFFER_SIZE);
			if (u8_returnValueUART != 0)
			{
				_u8_gpsState++;
			}
			break;

		case 1:
			if ((_tUartRxBuffer[_cUartRxCounter] != '\n') && (_cUartRxCounter < BUFFER_SIZE))
			{
				_bTrameNmeaBuffer = _RUN_GPS_getNmeaBuffer(pBuffer, _tUartRxBuffer[_cUartRxCounter]);
				_cUartRxCounter++;
			}
			else if (_cUartRxCounter >= BUFFER_SIZE)
			{
				for (size_t i = 0; i < BUFFER_SIZE; i++)
				{
					_tUartRxBuffer[i] = 0;
				}
				_cUartRxCounter = 0;
				_u8_gpsState = 0;
			}
			else
			{
				_bTrameNmeaBuffer = _RUN_GPS_getNmeaBuffer(pBuffer, _tUartRxBuffer[_cUartRxCounter]);
				if (_bTrameNmeaBuffer != 0)
				{
					_u8_gpsState++;
				}
				else
				{
					for (size_t i = 0; i < BUFFER_SIZE; i++)
					{
						_tUartRxBuffer[i] = 0;
					}
					_cUartRxCounter = 0;
					_u8_gpsState = 0;
				}	
			}
			break;

		case 2:
			_bDecodeNmeaBuffer = _RUN_GPS_decodeNmeaBuffer(pBuffer, pNmeaRmc);
			for (size_t i = 0; i < BUFFER_SIZE; i++)
			{
				_tUartRxBuffer[i] = 0;
			}
			_cUartRxCounter = 0;
			_u8_gpsState = 0;
			break;

		default:
			break;
	}

	return _bDecodeNmeaBuffer;
}

static uint8_t _RUN_GPS_decodeNmeaBuffer(BufferNmea *pBuffer, DataNmea_RMC *pNmeaRmc) {
	uint8_t _bDecodeRmc = 0;
	char *ptr = &pBuffer->data[3];
	
	if(!(strncmp(ptr, "RMC", 3))) {
		_bDecodeRmc = _RUN_GPS_decodeNmeaRmc(pBuffer, pNmeaRmc);
	}
	
	return _bDecodeRmc;
}

static uint8_t _RUN_GPS_decodeNmeaRmc(BufferNmea *pBuffer, DataNmea_RMC *pNmeaRmc) {
	uint8_t _bDecodeRmc = 0;
	uint8_t _cDataBufferCounter = 0;
	uint8_t _cDataFieldCounter = 0;
	
	pBuffer->nmea = RMC_MESSAGE;
	
	while((pBuffer->data[_cDataBufferCounter] != '*') || (_cDataBufferCounter >= BUFFER_SIZE)) {
		switch(pBuffer->nmea) {
			case RMC_MESSAGE:
				if(pBuffer->data[_cDataBufferCounter] == ',') {
					pBuffer->nmea = RMC_UTC_TIME;
				}
				break;
				
			case RMC_UTC_TIME:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->utcTime[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_STATUS;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_STATUS:
				if(pBuffer->data[_cDataBufferCounter] == ',') {
					pBuffer->nmea = RMC_LAT;
				}
				break;
					
			case RMC_LAT:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->latitude[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_LAT_DIR;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_LAT_DIR:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->latitudeDir = pBuffer->data[_cDataBufferCounter];
				}
				else {
					pBuffer->nmea = RMC_LONG;
				}
				break;
				
			case RMC_LONG:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->longitude[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_LONG_DIR;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_LONG_DIR:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->longitudeDir = pBuffer->data[_cDataBufferCounter];
				}
				else {
					pBuffer->nmea = RMC_SPEED;
				}
				break;
				
			case RMC_SPEED:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->speed[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_CAP;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_CAP:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->cap[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_UTC_DATE;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_UTC_DATE:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->utcDate[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_DECL_MAGN;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_DECL_MAGN:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->declMagn[_cDataFieldCounter] = pBuffer->data[_cDataBufferCounter];
					_cDataFieldCounter++;
				}
				else {
					pBuffer->nmea = RMC_DECL_MAGN_DIR;
					_cDataFieldCounter = 0;
				}
				break;
				
			case RMC_DECL_MAGN_DIR:
				if(pBuffer->data[_cDataBufferCounter] != ',') {
					pNmeaRmc->declMagnDir = pBuffer->data[_cDataBufferCounter];
					pBuffer->nmea = RMC_MODE;
					_bDecodeRmc = 1;
				}
				break;
				
			case RMC_MODE:
				break;
				
			default:
				pBuffer->nmea = RMC_MESSAGE;
				_cDataBufferCounter = 0;
				_cDataFieldCounter = 0;
		}
		_cDataBufferCounter++;
	}
	
	return _bDecodeRmc;
}

static uint8_t _RUN_GPS_getNmeaChecksum(char *dataChecksum) 
{
    char checksum = 0;
    char values[3];
    uint8_t flagStartData = 0;
    uint8_t indiceStartChecksum = 0;
    
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if ((dataChecksum[i] == '$' || flagStartData) && dataChecksum[i] != '*') {
            if (dataChecksum[i] == '$') {
                i++;
            }
            flagStartData = 1;
            checksum ^= dataChecksum[i];
        }
        else if (dataChecksum[i] == '*') {
            flagStartData = 0;
            indiceStartChecksum = i;
            i = BUFFER_SIZE;
        }
    }
    
    sprintf(values, "%X", checksum);
    if (values[0] == dataChecksum[indiceStartChecksum+1] && values[1] == dataChecksum[indiceStartChecksum+2]) {
        return 1;
    }
    else
        return 0;
}

static uint8_t _RUN_GPS_getNmeaBuffer(BufferNmea *pBuffer, char byte) 
{
    uint8_t packetNmea = 0;
    
    if (byte == '$') {
        pBuffer->indice = 0;
        pBuffer->nmea = NMEA_START;
    }
    
    switch (pBuffer->nmea) {
        case NMEA_START:
            if (byte == '$') {
                pBuffer->data[0] = '$';
                pBuffer->nmea = NMEA_G;
            }
            break;
            
        case NMEA_G:
            if (byte == 'G') {
                pBuffer->data[1] = 'G';
                pBuffer->nmea = NMEA_P_A_L_N;
            }
            else {
                pBuffer->nmea = NMEA_START;
            }
            break;
        
        case NMEA_P_A_L_N:
            if (byte == 'P' || byte == 'A' || byte == 'L' || byte == 'N') {
                pBuffer->data[2] = byte;
                pBuffer->indice = 3;
                pBuffer->nmea = NMEA_DATA;
            }
            else {
                pBuffer->nmea = NMEA_START;
            }
            break;
        
        case NMEA_DATA:
            if (byte == '\r') {
                pBuffer->nmea = NMEA_END;
                packetNmea = 1;
            }
            else {
                pBuffer->data[pBuffer->indice++] = byte;
            }
            break;
            
        case NMEA_END:
            if (byte == '\n') {
                pBuffer->data[pBuffer->indice] = 0;
                packetNmea = _RUN_GPS_getNmeaChecksum(pBuffer->data);
            }
            break;
            
        default:
            pBuffer->indice = 0;
            pBuffer->nmea = NMEA_START;
            break;
    }
    
    if (pBuffer->indice >= BUFFER_SIZE) {
        pBuffer->indice = 0;
        pBuffer->nmea = NMEA_START;
    }
    
    return packetNmea;
}

static void _RUN_GPS_decodeNmeaForMaster(DataNmea_RMC *pNmeaRmc) 
{
	_RUN_GPS_rmcUtcTime(pNmeaRmc);
	_RUN_GPS_rmcDate(pNmeaRmc);
	_RUN_GPS_rmcLatLong(pNmeaRmc);
}

static void _RUN_GPS_rmcUtcTime(DataNmea_RMC *pNmeaRmc)
{
    char tabTemp[3] = {0,0,0};
    
    tabTemp[0] = pNmeaRmc->utcTime[0];
    tabTemp[1] = pNmeaRmc->utcTime[1];
    gu8_hoursGpsAcquisition = (atoi(tabTemp));
    
    tabTemp[0] = pNmeaRmc->utcTime[2];
    tabTemp[1] = pNmeaRmc->utcTime[3];
    gu8_minutesGpsAcquisition = (atoi(tabTemp));
}

static void _RUN_GPS_rmcDate(DataNmea_RMC *pNmeaRmc)
{
    char tabTemp[3] = {0,0,0};

    tabTemp[0] = pNmeaRmc->utcDate[0];
    tabTemp[1] = pNmeaRmc->utcDate[1];
    gu8_daysGpsAcquisition = (atoi(tabTemp));

    tabTemp[0] = pNmeaRmc->utcDate[2];
    tabTemp[1] = pNmeaRmc->utcDate[3];
    gu8_monthsGpsAcquisition = (atoi(tabTemp));
}

static void _RUN_GPS_rmcLatLong(DataNmea_RMC *pNmeaRmc) 
{
    /*
     * Format lat ddmm.mmmmm -> dd uint8_t ; mm uint8_t ; mmmmm uint32_t  (uint32)MSB << 16 | (uint32)MiSB << 8 | (uint32)LSB
     * Format long dddmm.mmmmm -> ddd uint8_t ; mm uint8_t ; mmmmm uint32_t (uint32)MSB << 16 | (uint32)MiSB << 8 | (uint32)LSB
     */
	char latitudeDegrees[3] = { 0 };
    char latitudeMinutes[3] = { 0 };
    char latitudeDecimal[6] = { 0 };
    char longitudeDegrees[4] = { 0 };
    char longitudeMinutes[3] = { 0 };
    char longitudeDecimal[6] = { 0 };
    uint32_t decimalTemp;
	
    latitudeDegrees[0] = pNmeaRmc->latitude[0];
    latitudeDegrees[1] = pNmeaRmc->latitude[1];
    
    latitudeMinutes[0] = pNmeaRmc->latitude[2];
    latitudeMinutes[1] = pNmeaRmc->latitude[3];
    
    latitudeDecimal[0] = pNmeaRmc->latitude[5];
    latitudeDecimal[1] = pNmeaRmc->latitude[6];
    latitudeDecimal[2] = pNmeaRmc->latitude[7];
    latitudeDecimal[3] = pNmeaRmc->latitude[8];
    latitudeDecimal[4] = pNmeaRmc->latitude[9];
    
	guf_latitude.f_coordinates = ((float)(atoi(latitudeDegrees))) + (((float)(atoi(latitudeMinutes))) + ((float)(atoi(latitudeDecimal)) / 100000.0f)) / 60.0f;
	if (pNmeaRmc->latitudeDir == 'S')
	{
        guf_latitude.f_coordinates = -guf_latitude.f_coordinates;
	}

    longitudeDegrees[0] = pNmeaRmc->longitude[0];
    longitudeDegrees[1] = pNmeaRmc->longitude[1];
    longitudeDegrees[2] = pNmeaRmc->longitude[2];
    
    longitudeMinutes[0] = pNmeaRmc->longitude[3];
    longitudeMinutes[1] = pNmeaRmc->longitude[4];
    
    longitudeDecimal[0] = pNmeaRmc->longitude[6];
    longitudeDecimal[1] = pNmeaRmc->longitude[7];
    longitudeDecimal[2] = pNmeaRmc->longitude[8];
    longitudeDecimal[3] = pNmeaRmc->longitude[9];
    longitudeDecimal[4] = pNmeaRmc->longitude[10];

	guf_longitude.f_coordinates = ((float)(atoi(longitudeDegrees))) + (((float)(atoi(longitudeMinutes))) + ((float)(atoi(longitudeDecimal)) / 100000.0f)) / 60.0f;
	if (pNmeaRmc->longitudeDir == 'W')
	{
        guf_longitude.f_coordinates = -guf_longitude.f_coordinates;
	}
}

uint8_t RUN_GPS_GetHours(void)
{
    return gu8_hoursGpsAcquisition;
}

uint8_t RUN_GPS_GetMinutes(void)
{
    return gu8_minutesGpsAcquisition;
}

uint8_t RUN_GPS_GetDays(void)
{
    return gu8_daysGpsAcquisition;
}

uint8_t RUN_GPS_GetMonths(void)
{
    return gu8_monthsGpsAcquisition;
}

uint8_t RUN_GPS_GetLatitudeMMSB(void) 
{ 
	return (uint8_t)(guf_latitude.u32_coordinates >> 24);
}

uint8_t RUN_GPS_GetLatitudeMSB(void)   
{ 
	return (uint8_t)(guf_latitude.u32_coordinates >> 16);
}

uint8_t RUN_GPS_GetLatitudeLSB(void)   
{ 
	return (uint8_t)(guf_latitude.u32_coordinates >> 8);
}

uint8_t RUN_GPS_GetLatitudeLLSB(void)  
{ 
	return (uint8_t)(guf_latitude.u32_coordinates);
}

uint8_t RUN_GPS_GetLongitudeMMSB(void) 
{ 
	return (uint8_t)(guf_longitude.u32_coordinates >> 24);
}

uint8_t RUN_GPS_GetLongitudeMSB(void)  
{ 
	return (uint8_t)(guf_longitude.u32_coordinates >> 16);
}

uint8_t RUN_GPS_GetLongitudeLSB(void)  
{ 
	return (uint8_t)(guf_longitude.u32_coordinates >> 8);
}

uint8_t RUN_GPS_GetLongitudeLLSB(void) 
{ 
	return (uint8_t)(guf_longitude.u32_coordinates);
}
