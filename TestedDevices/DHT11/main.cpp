#include "DHT11.h"
#include <wiringPi.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <thread>

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
	dht11->setCallBack(&cb);
	dht11->start();
	getchar();
	dht11->stop();
	delete dht11;
	return 0;
}
