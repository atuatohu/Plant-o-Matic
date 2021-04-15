#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <thread>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <thread>

class UltrasonicCallback{
public:
	virtual void hasSample(int d) = 0;
};

class Ultrasonic{
public:
	Ultrasonic(int echo, int trig);
	~Ultrasonic(){
		stop();
	}
	void start();
	void stop();
	void setCallBack(UltrasonicCallback* cb);
private:	
	UltrasonicCallback* ultrasonicCb = NULL;
	void setPin(int e, int t);
	int echo, trig;
	int getDistanceCM();
	std::thread* ultraThread = NULL;
	static void run(Ultrasonic* ultrasonic);
	int running = 0;
};
#endif
