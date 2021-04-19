#include "ultrasonic.h"
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

std::mutex mtx;

int test_flag = 0;
const int pump = 0;

class UltraSonicSensorSampleCallback : public UltrasonicCallback{

  virtual void hasSample(int d){
      	mtx.lock();
    if(d > 3){
        test_flag += 1;
        digitalWrite(pump, 0);
		cout<<"Data "<<test_flag<<" received"<<endl;
	}
	mtx.unlock();  
    }       
};

int main(){
	if ( wiringPiSetup() == -1 )
		exit( 1 );
	int trig = 4;
	int echo = 5;
	Ultrasonic* ultra = new Ultrasonic(echo, trig);
	UltraSonicSensorSampleCallback cb;
	ultra->setCallBack(&cb);
	ultra->start();
	while(1){
	    if (test_flag == 5 && digitalRead(pump) == 0){
			cout<<"Unit test for Ultrasonic Sensor: 5 data required "<<test_flag<<" received"<<endl;
			ultra->stop();
			break;
	    }
	}
	ultra->stop();
	delete ultra;
	return 0;	
}
