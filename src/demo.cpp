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
#include "ultrasonic.h"
#include "MCP3008.h"
#include "DHT11.h"

class UltraSonicSensorSampleCallback:public UltrasonicCallback{
public:
	virtual void hasSample(int d){
		printf("level: %d\n", d);
	}
};
class DHT11SampleCallBack:public DHT11callback{
	//call back function that calculates humidity and temperature
	//from bits
public:
	float humidity,temperature;
	virtual void hasSample(int *data){
		humidity = (float) ((data[0] << 8) + data[1]) / 10.0;
		if (humidity > 100) humidity = data[0];
		temperature = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (temperature > 125) temperature = data[2];
		if (data[2] & 0x80) temperature = -temperature;
		float f_temperature = temperature * 9. / 5. + 32;
		printf("Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", humidity, temperature, f_temperature); 
	}
};
class SoilSensorSampleCallBack:public MCP3008callback {
public:
	int currentMoist;
	virtual void hasSample(int s){
		currentMoist = (s * 100) / 980;
		printf("soil = %d\n",currentMoist);
	}
};


int main(){
	if ( wiringPiSetup() == -1 )
		exit( 1 );
	const int pin = 2;
	const int trig = 4;
	const int echo = 5;
	DHT11* dht11 = new DHT11(pin);
	DHT11SampleCallBack cb1;
	MCP3008* soil_sensor = new MCP3008();
	SoilSensorSampleCallBack print1;
	uint8_t soil_channel = 0;
	soil_sensor->set_channel(soil_channel);
	soil_sensor->setCallBack(&print1);
	dht11->setCallBack(&cb1);
	Ultrasonic* ultra = new Ultrasonic(echo, trig);
	UltraSonicSensorSampleCallback cb2;
	
	ultra->setCallBack(&cb2);
	ultra->start();
	getchar();
	ultra->stop();
	dht11->stop();
	soil_sensor->stop();
	delete ultra;
	delete dht11;
	delete soil_sensor;
	return 0;
}
