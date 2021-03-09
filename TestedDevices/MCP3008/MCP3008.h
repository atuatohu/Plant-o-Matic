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

class MCP3008{
	public:
		void set_channel(uint8_t channel);
		void read_data();
	private:
		uint8_t adc_channel;
};

#endif
