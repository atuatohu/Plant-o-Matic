#include "MCP3008.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <unistd.h>
#define _USE_MATHS_DEFINES
#include <math.h>
#include <cstdio>
#include <mutex>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <regex>
#include <mutex>
using namespace std;
int test_flag = 0;
const int pump = 0;
int isOn = 0;

class SoilSensorSampleCallBack:public MCP3008callback {
public:
	virtual void hasSample(int s){
		pinMode(pump, OUTPUT);
		if (s > 800) {
			test_flag+=1;
			digitalWrite(pump, 1);
			if(digitalRead(pump) == 1){ 
				isOn +=1;
				cout << "pump is working "<< isOn<< " times"<<endl;
			}
		}
	}

};

int main(){	
	if ( wiringPiSetup() == -1 )
		exit( 1 );
	MCP3008* soil_sensor = new MCP3008();
	SoilSensorSampleCallBack print1;
	uint8_t soil_channel = 0;
	soil_sensor->set_channel(soil_channel);
	soil_sensor->setCallBack(&print1);
	
	soil_sensor->start();
	while(1){
		if (test_flag == 10 && isOn == 10){
			cout << "unit test for soil capacity sensor: 10 correct data required " <<test_flag<<" received"<<endl;
			soil_sensor->stop();
			break; 
		}
	}
	digitalWrite(pump, LOW);
	soil_sensor->stop();
	delete soil_sensor;
	return 0;
	
}
