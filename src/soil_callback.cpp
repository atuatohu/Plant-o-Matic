#include "global_variables.h"
#include "MCP3008.h"
#include <cstdio>

//handler which receives data and calculate soil moist
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
