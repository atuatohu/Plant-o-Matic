#include "ultrasonic.h"
#include "global_variables.h"
#include <cstdio>
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
