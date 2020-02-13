#include <stdio.h>
#include <util/delay.h>

#include "constant.h"
#include "initialisation.h"
#include "sensors.h"

int main(void) {
    Initialisation();
    
    while (1)
    {
        if(!(isDocking()))
        {
            startSensors();
        }
    }
    return 0; // never reached
}
