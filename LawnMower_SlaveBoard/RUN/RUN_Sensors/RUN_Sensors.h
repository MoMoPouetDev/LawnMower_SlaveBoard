/*
 * RUN_Sensors.h
 *
 *  Created on: 25 FEB 2023
 *      Author: morgan.venandy
 */

#ifndef RUN_RUN_SENSORS_RUN_SENSORS_H_
#define RUN_RUN_SENSORS_RUN_SENSORS_H_

/*--------------------------------------------------------------------------*/
/*! ... INCLUDES ...                                                        */
/*--------------------------------------------------------------------------*/
#include "stdint.h"
#include "HAL_GPIO.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
/*** Time to Mow ***/
#define THRESHOLD_HOUR_MIN 9
#define THRESHOLD_HOUR_MAX 18
/*** Capteur Tension ***/
#define CHARGING_THRESHOLD 600
#define SENSOR_V_OK 80
#define SENSOR_V_FAIBLE_WARN 20
#define SENSOR_V_FAIBLE_ERR 10
#define SENSOR_V_EMPTY 1
/*** SONAR ***/
#define SONAR_WARN 30
#define SONAR_LIMITE 20
#define SONAR_ERR 10
#define SONAR_DIST_ERR 999
#define TIMER0_OVERFLOW 255
/*** Calcul of value timer 343 m/s -> 34300 cm/s
 dist = (speedSound*TIMER)/2 = (34300*TIMER)/2 = 17150*TIMER = 17150 * (TIMER_VALUE * 0.125 * 10^-6) // 8MHz
  dist = (speedSound*TIMER)/2 = (34300*TIMER)/2 = 17150*TIMER = 17150 * (TIMER_VALUE * 0.05 * 10^-6) // 20MHz
 dist = (speedSound*TIMER)/2 = (34300*TIMER)/2 = 17150*TIMER = 17150 * (TIMER_VALUE * 0.017 * 10^-6) // 60MHz
 ***/
//#define TIMER_DISTANCE 466.47 //8MHz
#define TIMER_DISTANCE 1166.18 //20MHz
//#define TIMER_DISTANCE 29.15 //60MHz
#define THRESHOLD_8_BITS 0xFE
#define ERROR_DATA 0xFF

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
void RUN_Sensors_Init(void);
void RUN_Sensors_IsTimeToMow(uint8_t u8_hours);
void RUN_Sensors_IsCharging(uint16_t u16_chargeValue);
void RUN_Sensors_GetBatteryPercent(uint16_t uTension);
Etat RUN_Sensors_GetRainState(void);
Etat RUN_Sensors_GetDockState(void);
void RUN_Sensors_SetRainState(Etat e_rainState);
void RUN_Sensors_SonarDistance(void);
uint8_t RUN_Sensors_SonarGetDistanceFC(void);
uint8_t RUN_Sensors_SonarGetDistanceFL(void);
uint8_t RUN_Sensors_SonarGetDistanceFR(void);

#endif /* RUN_RUN_SENSORS_RUN_SENSORS_H_ */
