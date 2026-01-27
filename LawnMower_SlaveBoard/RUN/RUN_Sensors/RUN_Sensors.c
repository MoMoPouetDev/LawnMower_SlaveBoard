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
#include "RUN_I2C.h"
/*--------------------------------------------------------------------------*/
/* ... DATATYPES ...                                                        */
/*--------------------------------------------------------------------------*/
static uint8_t* gpu8_timeToMow;
static uint8_t* gpu8_isCharging;
static uint8_t* gpu8_batteryPurcent;
static uint8_t* gpu8_distanceSonarFC;
static uint8_t* gpu8_distanceSonarFL;
static uint8_t* gpu8_distanceSonarFR;
static volatile uint32_t _uTimerOvfCount;
static Etat* gpe_rain;
static Etat* gpe_dock;

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
    gpu8_timeToMow = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_TIME_TO_MOW);
    gpu8_isCharging = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_A);
    gpu8_batteryPurcent = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_V);
    gpu8_distanceSonarFC = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_SONAR_FC);
    gpu8_distanceSonarFL = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_SONAR_FL);
    gpu8_distanceSonarFR = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_SONAR_FR);
	*gpe_rain = OFF;//RUN_I2C_GetPointer(E_SLAVE_READ_DATA_RAIN);
	gpe_dock = RUN_I2C_GetDataPointer(E_SLAVE_READ_DATA_DOCK);

	_RUN_Sensors_SonarInit();
}

void RUN_Sensors_IsTimeToMow(uint8_t u8_hours)
{
	if ((THRESHOLD_HOUR_MIN <= u8_hours) && (u8_hours < THRESHOLD_HOUR_MAX))
	{
		*gpu8_timeToMow = 1;
	}
    else
    {
        *gpu8_timeToMow = 0;
    }
}

void RUN_Sensors_IsCharging(uint16_t u16_chargeValue)
{
	if (u16_chargeValue <= CHARGING_THRESHOLD)
	{
		*gpu8_isCharging = 0;
	}
	else
	{
		*gpu8_isCharging = 1;
	}
}

/**********************************************/
//	0%   | 9     | 1,8   | 368,0981595
//	5%   | 9,9   | 1,98  | 404,9079755
//	10%  | 10,8  | 2,16  | 441,7177914
//	15%  | 10,95 |  2,19 | 447,8527607
//	20%  | 11,1  | 2,22  | 453,9877301
//	25%  | 11,18 | 2,236 | 457,2597137
//	30%  | 11,25 | 2,25  | 460,1226994
//	40%  | 11,37 | 2,274 | 465,0306748
//	50%  | 11,49 | 2,298 | 469,9386503
//	60%  | 11,61 | 2,322 | 474,8466258
//	70%  | 11,76 | 2,352 | 480,9815951
//	75%  | 11,84 | 2,368 | 484,2535787
//	80%  | 11,91 | 2,382 | 487,1165644
//	85%  | 12,1  | 2,42  | 494,8875256
//	90%  | 12,3  | 2,46  | 503,0674847
//	95%  | 12,45 | 2,49  | 509,202454
//	100% | 12,6  | 2,52  | 515,3374233
/*********************************************/
void RUN_Sensors_GetBatteryPercent(uint16_t uTension) 
{
	uint8_t uPourcentage;
	
	if(uTension < 370) { uPourcentage = 0; }
	else if(uTension >= 370 && uTension < 405) { uPourcentage = 5; }
	else if(uTension >= 405 && uTension < 442) { uPourcentage = 10; }
	else if(uTension >= 442 && uTension < 448) { uPourcentage = 15; }
	else if(uTension >= 448 && uTension < 454) { uPourcentage = 20; }
	else if(uTension >= 454 && uTension < 457) { uPourcentage = 25; }
	else if(uTension >= 457 && uTension < 460) { uPourcentage = 30; }
	else if(uTension >= 460 && uTension < 465) { uPourcentage = 40; }
	else if(uTension >= 465 && uTension < 470) { uPourcentage = 50; }
	else if(uTension >= 470 && uTension < 475) { uPourcentage = 60; }
	else if(uTension >= 475 && uTension < 481) { uPourcentage = 70; }
	else if(uTension >= 481 && uTension < 484) { uPourcentage = 75; }
	else if(uTension >= 484 && uTension < 487) { uPourcentage = 80; }
	else if(uTension >= 487 && uTension < 495) { uPourcentage = 85; }
	else if(uTension >= 495 && uTension < 503) { uPourcentage = 90; }
	else if(uTension >= 503 && uTension < 509) { uPourcentage = 95; }
	else if(uTension >= 509) { uPourcentage = 100; }
	else { uPourcentage = 100; }
	
	*gpu8_batteryPurcent = uPourcentage;
}

Etat RUN_Sensors_GetRainState(void)
{
	return *gpe_rain;
}

void RUN_Sensors_SetRainState(Etat e_rainState)
{
	*gpe_rain = e_rainState;
}

Etat RUN_Sensors_GetDockState(void)
{
	return *gpe_dock;
}

void RUN_Sensors_SetDockState(Etat e_dockState)
{
	*gpe_dock = e_dockState;
}

static void _RUN_Sensors_SonarInit(void) 
{
    *gpu8_distanceSonarFC = 255;
    *gpu8_distanceSonarFL = 255;
    *gpu8_distanceSonarFR = 255;
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
			_RUN_Sensors_SonarDistanceCalculation(E_CENTER_ECHO, &u32_distance);
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
            *gpu8_distanceSonarFC = RUN_FIFO_GetSonarAverageFC(_u8_distanceSonarFC);
            *gpu8_distanceSonarFL = RUN_FIFO_GetSonarAverageFL(_u8_distanceSonarFL);
            *gpu8_distanceSonarFR = RUN_FIFO_GetSonarAverageFR(_u8_distanceSonarFR);
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
    for (size_t i = 0; i < 30; i++);    
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
                _u8_echoState++;
            }
            break;

        case 1:
            u8_echoPinState = HAL_GPIO_ReadPinSonar(e_gpio);
            if (u8_echoPinState == 0)
            {
                u32_timerValue = TCNT0 + (TIMER0_OVERFLOW*_uTimerOvfCount);
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
    return *gpu8_distanceSonarFC;
}

uint8_t RUN_Sensors_SonarGetDistanceFL(void)
{
    return *gpu8_distanceSonarFL;
}

uint8_t RUN_Sensors_SonarGetDistanceFR(void)
{
    return *gpu8_distanceSonarFR;
}
