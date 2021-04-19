#include "DHT11.h"
#include <cstdio>


class DHT11SampleCallBack:public DHT11callback{
	//call back function that calculates humidity and temperature
	//from bits
public:
	float humidity,temperature;
	long t; 
	virtual void hasSample(int *data){
		humidity = (float) ((data[0] << 8) + data[1]) / 10.0;
		if (humidity > 100) humidity = data[0];
		temperature = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (temperature > 125) temperature = data[2];
		if (data[2] & 0x80) temperature = -temperature;
		float f_temperature = temperature * 9. / 5. + 32;
		t = time(NULL);
		printf("Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", humidity, temperature, f_temperature); 
	}
};
