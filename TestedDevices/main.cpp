#include "MCP3008.h"
#include "DHT11.h"
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

class DHT11SampleCallBack:public DHT11callback{
	//call back function that calculates humidity and temperature
	//from bits
	virtual void hasSample(int *data){
		float humidity = (float) ((data[0] << 8) + data[1]) / 10.0;
		if (humidity > 100) humidity = data[0];
		float temperature = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (temperature > 125) temperature = data[2];
		if (data[2] & 0x80) temperature = -temperature;
		float f_temperature = temperature * 9. / 5. + 32;
		printf("Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", humidity, temperature, f_temperature); 
	}
};

int main(int argc, char *argv[]){
        if ( wiringPiSetup() == -1 )
		exit( 1 );
	int pin = 7;
	DHT11* dht11 = new DHT11(pin);
	DHT11SampleCallBack cb;
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
        dht11->setCallBack(&cb);
        soil_sensor->start();
        water_level_sensor->start();
        dht11->start();
        getchar();
        soil_sensor->stop();
        water_level_sensor->stop();
        dht11->stop();
        delete soil_sensor;
        delete water_level_sensor;
        delete dht11;
    return 0;
}
