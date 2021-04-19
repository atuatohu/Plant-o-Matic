#include "ultrasonic.h"

Ultrasonic::Ultrasonic(int echo, int trig){
	wiringPiSetup();
	setPin(echo, trig);
	pinMode(echo, INPUT);
	pinMode(trig, OUTPUT);
	digitalWrite(trig, LOW);
	delay(30);
}

void Ultrasonic::setPin(int e, int t){
		echo = e;
		trig = t;
}

void Ultrasonic::setCallBack(UltrasonicCallback* cb){
		ultrasonicCb = cb;
}
int Ultrasonic::getDistanceCM(){
		digitalWrite(trig, HIGH);
        delayMicroseconds(20);
        digitalWrite(trig, LOW);

        //Wait for echo start
        while(digitalRead(echo) == LOW);

        //Wait for echo end
        long startTime = micros();
        while(digitalRead(echo) == HIGH);
        long travelTime = micros() - startTime;

        //Get distance in cm
        int distance = travelTime / 58;

        return distance;
}

void Ultrasonic::run(Ultrasonic* ultrasonic){
	ultrasonic->running = 1;
	while(ultrasonic->running){
		int distance = ultrasonic->getDistanceCM();
		if(ultrasonic->ultrasonicCb){
				ultrasonic->ultrasonicCb->hasSample(distance);
		}
	}
	
}

void Ultrasonic::start(){
	if (ultraThread) throw "called while ultraThread is running";
	ultraThread = new std::thread(run, this);
}

void Ultrasonic::stop(){
	running = 0;
	if(ultraThread){
		ultraThread->join();
		delete ultraThread;
		ultraThread = NULL;
	}
}

