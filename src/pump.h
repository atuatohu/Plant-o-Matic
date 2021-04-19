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

class pump{
public:
	/**
	 * clas constructor
	**/
	pump(int relay);
	/**
	 * clas constructor
	**/
	~pump(){
		stop();
	}
	/**
	* function that allows to set the GPIO pin
	**/
	void setPin(int p);
	/**
	 * starts the pump logic
	 **/
	void start();
	/**
	 * stops the pump logic
	 **/
	void stop();
private:
	std::string state;
	static void run(pump* p);
	void onoff();
	int relay;
	int running = 0;
	std::thread* pumpThread = NULL;
};

#endif
