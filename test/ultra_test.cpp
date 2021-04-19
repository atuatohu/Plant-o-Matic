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

class UltraSonicSensorSampleCallback : public UltrasonicCallback{

  virtual void hasSample(int d){
      	mtx.lock();
        test_flag += 1;
	cout<<"Data "<<test_flag<<" received"<<endl;
	mtx.unlock();  
    }       
};


int main(){
	int trig = 4;
	int echo = 5;
	Ultrasonic* ultra = new Ultrasonic(echo, trig);
	UltraSonicSensorSampleCallback cb;
	ultra->setCallBack(&cb);
	ultra->start();
	while(1){
	    if (test_flag == 10){
		cout<<"Unit test for Ultrasonic Sensor: 10 data required "<<test_flag<<"data received"<<endl;
		ultra->stop();
		break;
	    }
	}
	ultra->stop();
	delete ultra;
	return 0;	
	
}
