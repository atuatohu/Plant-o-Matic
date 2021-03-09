#include "MCP3008.h"
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <math.h>
#include <time.h>

int main(){
	wiringPiSPISetup(0, 1000);
    MCP3008* soil_sensor = new MCP3008();
    uint8_t soil_channel = 0;
    soil_sensor->set_channel(soil_channel);
    MCP3008* level_sensor = new MCP3008();
    uint8_t level_channel = 1;
    level_sensor->set_channel(level_channel);
    for (;;){
        printf("soil ");
        soil_sensor->read_data();
        printf("water level ");
        level_sensor->read_data();
        delay(50);
    }
    
    return 0;
}
