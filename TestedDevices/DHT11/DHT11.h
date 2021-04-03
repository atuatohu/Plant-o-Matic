#ifndef DHT11_H
#define DHT11_H
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

class DHT11callback{
	public:
		virtual void hasSample(int *data) = 0;
};

class DHT11{
	public:
		DHT11(int pin); //class constructor
		~DHT11(){ //class destructor
			stop();
		}
		void start(); //start threads
		void stop(); //stop threads
		void setCallBack(DHT11callback* cb); //function that allows to threads to set callbacks
		void set_pin(int pin); //function that allows to set the GPIO pin
	private:
		DHT11callback* dht11callback = NULL;
		int DHTPIN;
		int running = 0;
		const int MAXTIMINGS = 85;
		static void run(DHT11* dht11);
		std::thread* dhtThread = NULL;
		bool read_dht_data();
		int dht11_dat[5] = { 0, 0, 0, 0, 0 };
};

#endif
