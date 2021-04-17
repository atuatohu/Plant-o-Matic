#include <stdint.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <math.h>
#include <time.h>
#include <thread>
#include <mutex>
#include "DHT11.h"
using namespace std;

std::mutex mtx;

int test_flag = 0;
class DHT11SampleCallBack:public DHT11callback{

public:
	float humidity,temperature;
	virtual void hasSample(int *data){

		humidity = (float) ((data[0] << 8) + data[1]) / 10.0;
		if (humidity > 100) humidity = data[0];
		temperature = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (temperature > 125) temperature = data[2];
		if (data[2] & 0x80) temperature = -temperature;
		float f_temperature = temperature * 9. / 5. + 32;
		test_flag += 1;
		cout << "Data " <<test_flag<<" received"<<endl;

	}

};

int main(){
	if ( wiringPiSetup() == -1 )
		exit( 1 );
	const int pin = 2;
	DHT11* dht11 = new DHT11(pin);
	DHT11SampleCallBack cb;
	dht11->setCallBack(&cb);
	dht11->start();
	while(1){
		if(test_flag == 10){
			cout << "Unit test for DHT11/DHT22: 10 data required " <<test_flag<<" received"<<endl;
			dht11->stop();
			break;
		}
	}
	dht11->stop();
	delete dht11;
	return 0;
}
