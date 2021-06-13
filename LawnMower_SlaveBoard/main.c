#include <stdio.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "constant.h"
#include "gps.h"
#include "initialisation.h"
#include "sensors.h"

int main(void) {
    Initialisation();
    
    while (1)
    {
        if(!(SENSORS_isDocking()))
        {
            SENSORS_startSensors();
			wdt_reset();
        }
		else
		{
			GPS_startGpsAcquisitionWhenDocking();
		}
    }
    return 0; // never reached
}
