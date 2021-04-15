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

class UltraSonicSensorSampleCallback:public UltrasonicCallback{
public:
	virtual void hasSample(int d){
		printf("level: %d\n", d);
	}
};

int main(){
	int trig = 4;
	int echo = 5;
	Ultrasonic* ultra = new Ultrasonic(echo, trig);
	UltraSonicSensorSampleCallback cb;
	ultra->setCallBack(&cb);
	ultra->start();
	getchar();
	ultra->stop();
	delete ultra;
	return 0;
}
