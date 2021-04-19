#include "global_variables.h"
#include "MCP3008.h"
#include <cstdio>

class SoilSensorSampleCallBack:public MCP3008callback{
public:
	int currentMoist;
	long t;
	virtual void hasSample(int s){
		currentMoist = (s * 100) / 980;
		soil = currentMoist;
		t = time(NULL);
		printf("soil = %d\n",currentMoist);
	}
};
//g++ main.cpp ultrasonic.cpp pump.cpp soil_callback.cpp ultrasonic.cpp DHT11.cpp MCP3008.cpp -o main -lwiringPi -lpthread -lfcgi -lcurl
