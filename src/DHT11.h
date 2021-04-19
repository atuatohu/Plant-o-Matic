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

/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/

class DHT11callback{
	public:
		/**
		 * Called after a sample has arrived.
		 **/
		virtual void hasSample(int *data) = 0;
};

/** 
 * This class reads data from DHT11/22 sensors in the background
 * and calls a callback whenever data is available
 **/
class DHT11{
	public:
	 /**
	 * clas constructor
	 **/
		DHT11(int pin); 
		~DHT11(){ 
			stop();
		}
		/**	
		 * Starts the data acquisition in the background and the 
		 * callback is called with new samples
		* */
		void start(); //start threads
		
		/** 
		 * Stops the data acquisition
		 **/
		void stop(); //stop threads
		
		/** 
		 * function that allows to threads to set callbacks
		 **/
		void setCallBack(DHT11callback* cb); 
		
		/**
		 * function that allows to set the GPIO pin
		 **/
		void set_pin(int pin); 
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
