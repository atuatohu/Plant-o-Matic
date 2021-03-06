#ifndef DHT11_H
#define DHT11_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

class DHT11{
	public:
		void read_dht_data();
		void set_pin(int pin);
	private:
		int DHTPIN;
		const int MAXTIMINGS = 85;
};

#endif
