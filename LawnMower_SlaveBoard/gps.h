//
//  gps.h
//  LawnMower_SlaveBoard
//
//  Created by morgan on 13/02/2020.
//  Copyright © 2020 morgan venandy. All rights reserved.
//

#ifndef gps_h
#define gps_h

#include "constant.h"

typedef enum {
    NMEA_START, //$
    NMEA_G,
    NMEA_P_A_L_N, // GPS, GALILEO, GLONNASS
    NMEA_DATA,
    NMEA_END, //\r
}PacketNmea;

typedef enum {
    RMC_MESSAGE,
    RMC_UTC_TIME,
    RMC_STATUS,
    RMC_LAT,
    RMC_LAT_DIR,
    RMC_LONG,
    RMC_LONG_DIR,
    RMC_SPEED,
    RMC_CAP,
    RMC_UTC_DATE,
    RMC_DECL_MAGN,
    RMC_DECL_MAGN_DIR,
    RMC_MODE
}PacketRmc;

typedef struct {
    char data[BUFFER_SIZE];
    uint8_t indice;
    unsigned char nmea;
}BufferNmea;

/*** Trame RMC ***/
/* $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68 */
/* type,UTC hhmmss, Alerte, Lat ddmm.mm, Dir N/S, Lon dddmm.mm, Dir E/W, vitesse Noeud, cap, Date jjmmaa, declinaison magnétique ddd.d, Dir E/W, checksum */
/*****************/

typedef struct {
    char utcTime[7];
    char latitude[15];
    char latitudeDir;
    char longitude[15];
    char longitudeDir;
    char speed[6];
    char cap[6];
    char utcDate[7];
    char declMagn[6];
    char declMagnDir;
}DataNmea_RMC;

typedef struct {
    uint8_t utcTimeHours;
    uint8_t utcTimeMinutes;
    uint8_t utcTimeSeconds;
    float latitude;
    float longitude;
    float speed;
    float cap;
    uint8_t utcDateDays;
    uint8_t utcDateMonths;
    uint8_t utcDateYears;
    float declMagn;
}DataNmea;

uint8_t startGpsAcquisition(void);
void initBufferNmea(BufferNmea*);
void initDataRmc(DataNmea_RMC*);
void initDataNmea(DataNmea*);
uint8_t decodeNmeaBuffer(BufferNmea*, DataNmea_RMC*, DataNmea*);
uint8_t decodeNmeaRmc(BufferNmea*, DataNmea_RMC*, DataNmea*);
uint8_t getNmeaUart(BufferNmea*, DataNmea_RMC*, DataNmea*);
uint8_t getNmeaChecksum(char*);
uint8_t getNmeaBuffer(BufferNmea*, char);

#endif /* gps_h */