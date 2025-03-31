/*
 * RUN_Sensors.c
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
#include "HAL_GPIO.h"
#include "HAL_ADC.h"
#include "RUN_Sensors.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
#define SONAR_DIST_ERR 999
#define TIMER1_OVERFLOW 65535
/*** Calcul of value timer 343 m/s -> 34300 cm/s
 dist = (speedSound*TIMER)/2 = (34300*TIMER)/2 = 17150*TIMER = 17150 * (TIMER_VALUE * 0.125 * 10^-6)
 ***/
#define TIMER_DISTANCE 466.47

static uint8_t gu8_distanceSonarFC;
static uint8_t gu8_distanceSonarFL;
static uint8_t gu8_distanceSonarFR;
static volatile uint16_t _uTimerOvfCount;
static Etat ge_rain;
static Etat ge_dock;

/*--------------------------------------------------------------------------*/
/*! ... LOCAL FUNCTIONS DECLARATIONS ...                                    */
/*--------------------------------------------------------------------------*/
static void _RUN_Sensors_SonarInit(void);
static void _RUN_Sensors_SonarSendPulse(GPIO e_gpio);
static uint8_t _RUN_Sensors_SonarDistanceCalculation(GPIO e_gpio, uint32_t *pu32_distance);
/*--------------------------------------------------------------------------*/
/*! ... FUNCTIONS DEFINITIONS    ...                                        */
/*--------------------------------------------------------------------------*/
ISR(TIMER0_OVF_vect)
{
    _uTimerOvfCount++;
}

void RUN_Sensors_Init(void)
{
	ge_rain = OFF;
	ge_dock = OFF;

	_RUN_Sensors_SonarInit();
}

void RUN_Sensors_IsTimeToMow(void)
{
	uint8_t u8_returnValue = 0;
	uint8_t u8_hours;

	u8_hours = RUN_GPS_GetHours();

	if ((THRESHOLD_HOUR_MIN <= u8_hours) && (u8_hours < THRESHOLD_HOUR_MAX))
	{
		u8_returnValue = 1;
	}

	return u8_returnValue;
}

uint8_t RUN_Sensors_IsCharging(void)
{
	uint8_t u8_returnValue = 0;
	uint32_t u8_chargeValue;

	u8_chargeValue = HAL_ADC_GetChargeValue();

	if (u8_chargeValue <= CHARGING_THRESHOLD)
	{
		u8_returnValue = 0;
	}
	else
	{
		u8_returnValue = 1;
	}

	return u8_returnValue;
}

/**********************************************/
//	0%   | 9     | 1,8   | 2235
//	5%   | 9,9   | 1,98  | 2458
//	10%  | 10,8  | 2,16  | 2681
//	15%  | 10,95 |  2,19 | 2718
//	20%  | 11,1  | 2,22  | 2756
//	25%  | 11,18 | 2,236 | 2775
//	30%  | 11,25 | 2,25  | 2793
//	40%  | 11,37 | 2,274 | 2823
//	50%  | 11,49 | 2,298 | 2852
//	60%  | 11,61 | 2,322 | 2882
//	70%  | 11,76 | 2,352 | 2919
//	75%  | 11,84 | 2,368 | 2939
//	80%  | 11,91 | 2,382 | 2957
//	85%  | 12,1  | 2,42  | 3004
//	90%  | 12,3  | 2,46  | 3053
//	95%  | 12,45 | 2,49  | 3091
//	100% | 12,6  | 2,52  | 3128
/*********************************************/
uint8_t RUN_Sensors_GetBatteryPercent(void) 
{
	uint32_t uTension;
	uint8_t uPourcentage = 0;

	uTension = HAL_ADC_GetBatteryValue();
	
	if(uTension <= 2300) { uPourcentage = 0; }
	else if(uTension <= 2458) { uPourcentage = 5; }
	else if(uTension <= 2681) { uPourcentage = 10; }
	else if(uTension <= 2718) { uPourcentage = 15; }
	else if(uTension <= 2756) { uPourcentage = 20; }
	else if(uTension <= 2775) { uPourcentage = 25; }
	else if(uTension <= 2793) { uPourcentage = 30; }
	else if(uTension <= 2823) { uPourcentage = 40; }
	else if(uTension <= 2852) { uPourcentage = 50; }
	else if(uTension <= 2882) { uPourcentage = 60; }
	else if(uTension <= 2919) { uPourcentage = 70; }
	else if(uTension <= 2939) { uPourcentage = 75; }
	else if(uTension <= 2957) { uPourcentage = 80; }
	else if(uTension <= 3004) { uPourcentage = 85; }
	else if(uTension <= 3053) { uPourcentage = 90; }
	else if(uTension <= 3091) { uPourcentage = 95; }
	else { uPourcentage = 100; }
	
	return uPourcentage;
}

Etat RUN_Sensors_GetRainState(void)
{
	return ge_rain;
}

void RUN_Sensors_SetRainState(Etat e_rainState)
{
	ge_rain = e_rainState;
}

Etat RUN_Sensors_GetDockState(void)
{
	return ge_dock;
}

void RUN_Sensors_SetDockState(Etat e_dockState)
{
	ge_dock = e_dockState;
}

static void _RUN_Sensors_SonarInit(void) 
{
    gu8_distanceSonarFC = 255;
    gu8_distanceSonarFL = 255;
    gu8_distanceSonarFR = 255;
}

void RUN_Sensors_SonarDistance(void)
{
    static uint8_t u8_sonarState = 0;
    uint8_t u8_echoState = 0;
    uint32_t u32_distance = 0;

    switch (u8_sonarState)
    {
        case 0:
            HAL_Sonar_SendPulse(E_CENTER_TRIGGER);
            u8_sonarState++;
            break;

        case 1:
            u8_echoState = HAL_Sonar_DistanceCalculation(E_CENTER_TRIGGER, &u32_distance);
            if (u8_echoState == 1)
            {
                gu8_distanceSonarFC = (uint8_t)u32_distance;
                u8_sonarState++;
            }
            break;

        case 2:
            HAL_Sonar_SendPulse(E_LEFT_TRIGGER);
            u8_sonarState++;
            break;

        case 3:
            u8_echoState = HAL_Sonar_DistanceCalculation(E_LEFT_TRIGGER, &u32_distance);
            if (u8_echoState == 1)
            {
                gu8_distanceSonarFL = (uint8_t)u32_distance;
                u8_sonarState++;
            }
            break;

        case 4:
            HAL_Sonar_SendPulse(E_RIGHT_TRIGGER);
            u8_sonarState = ++;
            break;

        case 5:
            u8_echoState = HAL_Sonar_DistanceCalculation(E_RIGHT_TRIGGER, &u32_distance);
            if (u8_echoState == 1)
            {
                gu8_distanceSonarFR = (uint8_t)u32_distance;
                u8_sonarState++;
            }
            break;

        default:
            u8_sonarState = 0;
            break;
    }
}

static void _RUN_Sensors_SonarSendPulse(GPIO e_gpio)
{       
    HAL_GPIO_WritePinSonar(e_gpio, 1);
    for (size_t i = 0; i < 100; i++);    
    HAL_GPIO_WritePinSonar(e_gpio, 0);
}

static uint8_t _RUN_Sensors_SonarDistanceCalculation(GPIO e_gpio, uint32_t *pu32_distance)
{
    static uint8_t _u8_echoState = 0;
    uint8_t u8_echoPinState = 0;
    uint8_t u8_returnValue = 0;
    uint32_t u32_timerValue = 0;

    switch (_u8_echoState)
    {
        case 0:
            u8_echoPinState = HAL_GPIO_ReadPinSonar(e_gpio);
            if (u8_echoPinState != 0)
            {
                TCNT0 = 0;
                _u8_echoState = 1;
            }
            break;

        case 1:
            u8_echoPinState = HAL_GPIO_ReadPinSonar(e_gpio);
            if (u8_echoPinState == 0)
            {
                u32_timerValue = TCNT1 + (TIMER1_OVERFLOW*_uTimerOvfCount);
                *pu32_distance = (u32_timerValue / TIMER_DISTANCE)/2;

                if(*pu32_distance > THRESHOLD_8_BITS) 
                {
                    *pu32_distance = THRESHOLD_8_BITS;
                }
                _uTimerOvfCount = 0;
                _u8_echoState = 0;
                u8_returnValue = 1;
            }
            break;
        
        default:
            break;
    }

    return u8_returnValue;
}

uint8_t RUN_Sensors_SonarGetDistanceFC(void)
{
    return gu8_distanceSonarFC;
}

uint8_t RUN_Sensors_SonarGetDistanceFL(void)
{
    return gu8_distanceSonarFL;
}

uint8_t RUN_Sensors_SonarGetDistanceFR(void)
{
    return gu8_distanceSonarFR;
}
