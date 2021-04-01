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
#include <thread>

class SoilSensorSampleCallBack:public MCP3008callback {
    virtual void hasSample(int s){
        printf("soil = %d\n",s);
    }
};

class WaterLevelSensorSampleCallBack:public MCP3008callback {
    virtual void hasSample(int w){
        printf("water level = %d\n",w);
    }
};

int main(int argc, char *argv[]){
        MCP3008* soil_sensor = new MCP3008();
        MCP3008* water_level_sensor = new MCP3008();
        SoilSensorSampleCallBack print1;
        WaterLevelSensorSampleCallBack print2;
        uint8_t soil_channel = 0;
        uint8_t water_channel = 2;
        soil_sensor->set_channel(soil_channel);
        water_level_sensor->set_channel(water_channel);
        soil_sensor->setCallBack(&print1);
        water_level_sensor->setCallBack(&print2);
        soil_sensor->start();
        water_level_sensor->start();
        getchar();
        soil_sensor->stop();
        water_level_sensor->stop();
        delete soil_sensor;
        delete water_level_sensor;
        /*MCP3008* soil_sensor = new MCP3008();
        SoilSensorSampleCallBack print1;
        uint8_t channel = 0;
        soil_sensor->set_channel(channel);
        soil_sensor->setCallBack(&print1);
        soil_sensor->start();
        getchar();
        soil_sensor->stop();
        delete soil_sensor;*/
    return 0;
}
