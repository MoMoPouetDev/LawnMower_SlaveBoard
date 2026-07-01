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
#include <avr/interrupt.h>
#include "HAL_GPIO.h"
#include "HAL_ADC.h"
#include "RUN_Sensors.h"
#include "RUN_FIFO.h"
#include "RUN_GPS.h"

/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
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

uint8_t RUN_Sensors_IsTimeToMow(void)
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
    uint16_t u16_chargeValue;

    u16_chargeValue = HAL_ADC_GetChargeValue();

    if (u16_chargeValue <= CHARGING_THRESHOLD)
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
    uint16_t uTension = 0;
    uint8_t uPourcentage = 0;

    uTension = HAL_ADC_GetBatteryValue();

	if(uTension < 370) { uPourcentage = 0; }
	else if(uTension >= 370 && uTension < 405) { uPourcentage = 5; }
	else if(uTension >= 405 && uTension < 442) { uPourcentage = 10; }
	else if(uTension >= 442 && uTension < 448) { uPourcentage = 15; }
	else if(uTension >= 448 && uTension < 454) { uPourcentage = 20; }
	else if(uTension >= 454 && uTension < 460) { uPourcentage = 25; }
	else if(uTension >= 460 && uTension < 465) { uPourcentage = 30; }
	else if(uTension >= 465 && uTension < 470) { uPourcentage = 40; }
	else if(uTension >= 470 && uTension < 475) { uPourcentage = 50; }
	else if(uTension >= 475 && uTension < 481) { uPourcentage = 60; }
	else if(uTension >= 481 && uTension < 484) { uPourcentage = 70; }
	else if(uTension >= 484 && uTension < 487) { uPourcentage = 75; }
	else if(uTension >= 487 && uTension < 495) { uPourcentage = 80; }
	else if(uTension >= 495 && uTension < 503) { uPourcentage = 85; }
	else if(uTension >= 503 && uTension < 509) { uPourcentage = 90; }
	else if(uTension >= 509 && uTension < 515) { uPourcentage = 95; }
	else if(uTension >= 515) { uPourcentage = 100; }
	
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
    static uint8_t _u8_sonarState = 0;
    static uint8_t _u8_distanceSonarFC = 255;
    static uint8_t _u8_distanceSonarFL = 255;
    static uint8_t _u8_distanceSonarFR = 255;
    uint8_t u8_echoState = 0;
    uint32_t u32_distance = 0;

    switch (_u8_sonarState)
    {
    case 0:
        _RUN_Sensors_SonarSendPulse(E_CENTER_TRIGGER);
        _u8_sonarState++;
        break;

    case 1:
        u8_echoState = _RUN_Sensors_SonarDistanceCalculation(E_CENTER_ECHO, &u32_distance);
        if (u8_echoState == 1)
        {
            _u8_distanceSonarFC = (uint8_t)u32_distance;
            _u8_sonarState++;
        }
        break;

    case 2:
        _RUN_Sensors_SonarSendPulse(E_LEFT_TRIGGER);
        _u8_sonarState++;
        break;

    case 3:
        u8_echoState = _RUN_Sensors_SonarDistanceCalculation(E_LEFT_ECHO, &u32_distance);
        if (u8_echoState == 1)
        {
            _u8_distanceSonarFL = (uint8_t)u32_distance;
            _u8_sonarState++;
        }
        break;

    case 4:
        _RUN_Sensors_SonarSendPulse(E_RIGHT_TRIGGER);
        _u8_sonarState++;
        break;

    case 5:
        u8_echoState = _RUN_Sensors_SonarDistanceCalculation(E_RIGHT_ECHO, &u32_distance);
        if (u8_echoState == 1)
        {
            _u8_distanceSonarFR = (uint8_t)u32_distance;
            _u8_sonarState++;
        }
        break;

    case 6:
        gu8_distanceSonarFC = RUN_FIFO_GetSonarAverageFC(_u8_distanceSonarFC);
        gu8_distanceSonarFL = RUN_FIFO_GetSonarAverageFL(_u8_distanceSonarFL);
        gu8_distanceSonarFR = RUN_FIFO_GetSonarAverageFR(_u8_distanceSonarFR);
        _u8_sonarState = 0;
        break;

    default:
        _u8_sonarState = 0;
        break;
    }
}

static void _RUN_Sensors_SonarSendPulse(GPIO e_gpio)
{
    HAL_GPIO_WritePinSonar(e_gpio, 1);
    for (volatile uint16_t i = 0; i < 200; i++);
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
            _uTimerOvfCount = 0;
            TCNT0 = 0;
            _u8_echoState = 1;
        }
        break;

    case 1:
        u8_echoPinState = HAL_GPIO_ReadPinSonar(e_gpio);
        if (u8_echoPinState == 0)
        {
            u32_timerValue = TCNT0 + (TIMER0_OVERFLOW * _uTimerOvfCount);
            *pu32_distance = (u32_timerValue / TIMER_DISTANCE) / 2;

            if (*pu32_distance > THRESHOLD_8_BITS)
            {
                *pu32_distance = THRESHOLD_8_BITS;
            }
            _uTimerOvfCount = 0;
            _u8_echoState = 0;
            u8_returnValue = 1;
        }
		else if (_uTimerOvfCount > 3000)
		{
			*pu32_distance = THRESHOLD_8_BITS;
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
