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

class MCP3008callback{
	public:
		virtual void hasSample(int sample) = 0;
};

class MCP3008{
	public:
		MCP3008(); //class constructor
		~MCP3008() { //class destructor
			stop();
		}
		void set_channel(uint8_t channel); //set channel of ADC from 0 to 7
		void setCallBack(MCP3008callback* cb); //function that allows to threads to set callbacks
		void start(); //start threads
		void stop(); //stop threads
	private:
		uint8_t adc_channel;
		MCP3008callback* mcp3008callback = NULL;
		int running = 0;
		std::thread* adcThread = NULL; 
		int readData();
		static void run(MCP3008* mcp3008);
};

#endif
