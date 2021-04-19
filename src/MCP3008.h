#ifndef MCP3008_H
#define MCP3008_H
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

/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/
 
class MCP3008callback{
	public:
		/**
		* Called after a sample has arrived.
		**/
		virtual void hasSample(int sample) = 0;
};
/** This class reads data from MCP3008 ADC in the background
 * and calls a callback whenever data is available
 **/
class MCP3008{
	public:
		 /**
		 * clas constructor
		 **/
		MCP3008(); 
		/**
		 * clas destructor
		 **/ 
		~MCP3008() { 
			stop();
		}
		/**
		 * function that allows to set channel
		 * of ADC from 0 to 7
		 **/
		void set_channel(uint8_t channel); 
		
		/** 
		 * function that allows to threads to set callbacks
		 **/
		void setCallBack(MCP3008callback* cb); 
		
		/**	Starts the data acquisition in the background and the 
		 * callback is called with new samples
		**/
		void start(); 
		/** 
		 * Stops the data acquisition
		 **/
		void stop(); 
	private:
		uint8_t adc_channel;
		MCP3008callback* mcp3008callback = NULL;
		int running = 0;
		std::thread* adcThread = NULL; 
		int readData();
		static void run(MCP3008* mcp3008);
};

#endif
