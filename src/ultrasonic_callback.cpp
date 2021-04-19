#include "ultrasonic.h"
#include "global_variables.h"
#include <cstdio>

//handler which receives data and prints it on the screen
class UltraSonicSensorSampleCallback:public UltrasonicCallback{
public:
	int waterlevel;
	//int height = 14;
	virtual void hasSample(int d){
		waterlevel = d;
		level = waterlevel;
		printf("level: %d\n", waterlevel);
	}
};
