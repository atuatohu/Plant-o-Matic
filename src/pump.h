#ifndef PUMP_H
#define PUMP_H
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
#include <mutex>
//#include "soil_callback.cpp"
//#include "global_variables.h"

class pumpCallback{
public:
	virtual void hasState(std::string s)=0;
};
class pump{
public:
	pump(int relay);
	~pump(){
		stop();
	}
	void setPin(int p);
	void start();
	void stop();
	void setCallback(pumpCallback* p);
	std::string on_off;
	std::string string_state();
	void changeState(std::string state, int cnt);
private:
	std::string state;
	pumpCallback* pcb = NULL;
	static void run(pump* p);
	void onoff();
	int relay;
	int running = 0;
	std::thread* pumpThread = NULL;
};

#endif
