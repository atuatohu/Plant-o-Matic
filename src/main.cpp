#include "MCP3008.h"
#include "DHT11.h"
#include "ultrasonic.h"
#include "json_fastcgi_web_api.h"
#include "pump.h"
#include "global_variables.h"
#include "soil_callback.cpp"
#include "ultrasonic_callback.cpp"
#include "dht11_callback.cpp"
#include "treshhold.h"
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
#include <mutex>
//Author Bernd Porr

int mainRunning = 1;
volatile int soil, level;

/**
 * Handler when the user has pressed ctrl-C
 * send HUP via the kill command.
 **/
void sigHandler(int sig) { 
	if((sig == SIGHUP) || (sig == SIGINT)) {
		mainRunning = 0;
	}
}


/** 
 * Sets a signal handler so that you can kill
 * the background process gracefully with:
 * kill -HUP <PID>
 **/
void setHUPHandler() {
	struct sigaction act;
	memset (&act, 0, sizeof (act));
	act.sa_handler = sigHandler;
	if (sigaction (SIGHUP, &act, NULL) < 0) {
		perror ("sigaction");
		exit (-1);
	}
	if (sigaction (SIGINT, &act, NULL) < 0) {
		perror ("sigaction");
		exit (-1);
	}
}
/**
 * Callback handler which returns data to the
 * nginx server. The current soil moisture
 * and the timestamp is transmitted to nginx and the
 * javascript application.
 **/
class JSONCGIMCP3008Callback : public JSONCGIHandler::GETCallback{	
private:
	SoilSensorSampleCallBack* cb;
public:
	JSONCGIMCP3008Callback(SoilSensorSampleCallBack* s){
		cb = s;
	}
	virtual std::string getJSONString(){
		JSONCGIHandler::JSONGenerator jsonGenerator;
		jsonGenerator.add("epoch",(long)time(NULL));
		jsonGenerator.add("soilmoisture",cb->currentMoist);
		return jsonGenerator.getJSON();
	}
		
};  
/**
 * Callback handler which returns data to the
 * nginx server. The current temperature
 * and the timestamp is transmitted to nginx and the
 * javascript application.
 **/
class JSONDHT11CallbackTemp : public JSONCGIHandler::GETCallback{
private:
	DHT11SampleCallBack* dht11;
public:
	JSONDHT11CallbackTemp(DHT11SampleCallBack* cb){
		dht11 = cb;
	}
	virtual std::string getJSONString(){
		JSONCGIHandler::JSONGenerator jsonGenerator;
		jsonGenerator.add("epoch",(long)time(NULL));
		jsonGenerator.add("temperature",dht11->temperature);
		return jsonGenerator.getJSON();
	}
};
/**
 * Callback handler which returns data to the
 * nginx server. The current humidity
 * and the timestamp is transmitted to nginx and the
 * javascript application.
 **/
class JSONDHT11CallbackHum : public JSONCGIHandler::GETCallback{
private:
	DHT11SampleCallBack* dht11;
public:
	JSONDHT11CallbackHum(DHT11SampleCallBack* cb){
		dht11 = cb;
	}
	virtual std::string getJSONString(){
		JSONCGIHandler::JSONGenerator jsonGenerator;
		jsonGenerator.add("epoch",(long)time(NULL));
		jsonGenerator.add("humidity",dht11->humidity);
		return jsonGenerator.getJSON();
	}
};
/**
 * Callback handler which returns data to the
 * nginx server. The current water level
 * and the timestamp is transmitted to nginx and the
 * javascript application.
 **/
class JSONUltraCallback : public JSONCGIHandler::GETCallback{
private:
	UltraSonicSensorSampleCallback* ultra;
public:
	JSONUltraCallback(UltraSonicSensorSampleCallback* cb){
		ultra = cb;
	}
	virtual std::string getJSONString(){
		JSONCGIHandler::JSONGenerator jsonGenerator;
		jsonGenerator.add("epoch",(long)time(NULL));
		jsonGenerator.add("level",ultra->waterlevel);
		return jsonGenerator.getJSON();
	}
};
int main(int argc, char *argv[]){
    if (wiringPiSetup() == -1)
		exit( 1 );
	const int dhtpin = 7;
	const int trig = 4;
	const int echo = 5;
	const int relay = 0;
	Ultrasonic* ultra = new Ultrasonic(echo, trig);
	UltraSonicSensorSampleCallback cb1;
	DHT11* dht11 = new DHT11(dhtpin);
	DHT11SampleCallBack cb2;
	pump* p = new pump(relay);
	MCP3008* soil_sensor = new MCP3008();
	SoilSensorSampleCallBack cb3;
	const uint8_t soil_channel = 0;
	soil_sensor->set_channel(soil_channel);
	soil_sensor->setCallBack(&cb3);
	dht11->setCallBack(&cb2);
	ultra->setCallBack(&cb1);
	JSONUltraCallback ju(&cb1);	JSONCGIHandler* fastcgiultra = new JSONCGIHandler(&ju, nullptr, "/tmp/ultrasonic");
	JSONCGIMCP3008Callback jc(&cb3);
	JSONCGIHandler* fastcgi = new JSONCGIHandler(&jc,nullptr, "/tmp/soilsocket");
	JSONDHT11CallbackTemp jcdht(&cb2);
	JSONCGIHandler* fastcgidht = new JSONCGIHandler(&jcdht, nullptr, "/tmp/tempsocket");
	JSONDHT11CallbackHum humdht(&cb2);
	JSONCGIHandler* fastcgihum = new JSONCGIHandler(&humdht, nullptr, "/tmp/humsocket");
	p->start();
	soil_sensor->start();
	dht11->start();
	ultra->start();
	setHUPHandler();
	fprintf(stderr,"'%s' up and running.\n",argv[0]);
	while (mainRunning) sleep(1);
	fprintf(stderr,"'%s' shutting down.\n",argv[0]);
	dht11->stop();
	soil_sensor->stop();
	ultra->stop();
	p->stop();
	delete p;
	delete soil_sensor;
	delete dht11;
	delete ultra;
	delete fastcgidht;
	delete fastcgihum;
	delete fastcgi;
	delete fastcgiultra;
    return 0;
}
