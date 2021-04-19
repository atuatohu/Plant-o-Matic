#include "MCP3008.h"
#include "DHT11.h"
#include "ultrasonic.h"
#include "json_fastcgi_web_api.h"
#include "pump.h"
#include "global_variables.h"
#include "soil_callback.cpp"
#include "ultrasonic_callback.cpp"
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

/*class UltraSonicSensorSampleCallback:public UltrasonicCallback{
public:
	int level;
	int height = 6;
	virtual void hasSample(int d){
		level = d;
		printf("level: %d\n", d);
	}
};*/

class DHT11SampleCallBack:public DHT11callback{
	//call back function that calculates humidity and temperature
	//from bits
public:
	float humidity,temperature;
	long t; 
	float forceTemp, forcedHum;
	int forcedCounter = 0;
	int forcedCounter2 = 0;
	virtual void hasSample(int *data){
		humidity = (float) ((data[0] << 8) + data[1]) / 10.0;
		if (humidity > 100) humidity = data[0];
		temperature = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if (temperature > 125) temperature = data[2];
		if (data[2] & 0x80) temperature = -temperature;
		float f_temperature = temperature * 9. / 5. + 32;
		t = time(NULL);
		printf("Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", humidity, temperature, f_temperature); 
	}
};

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
/*class pumpSample : public pumpCallback{
public:
	std::string state;
	virtual void hasState(std::string s){
		state = s;
		if(state == "on") digitalWrite(0, 1);
		else digitalWrite(0,0);
	}

	
};
class JSONpumpCallback : public JSONCGIHandler::GETCallback{
private:
	pumpSample* p;
public:
	JSONpumpCallback(pumpSample* relay){
		p = relay;
	}
	virtual std::string getJSONString() {
		JSONCGIHandler::JSONGenerator jsonGenerator;
		jsonGenerator.add("epoch",(long)time(NULL));
		jsonGenerator.add("state",p->state);
		return jsonGenerator.getJSON();
	}
};
class pumpPOSTCallback : public JSONCGIHandler::POSTCallback{
public:
	pumpSample* p;
	pumpPOSTCallback(pumpSample* relay){
		p = relay;
	}
	virtual void postString(std::string postArg) {
		auto m = JSONCGIHandler::postDecoder(postArg);
		std::string state = m["state"].c_str();
		int steps = atoi(m["steps"].c_str());
		std::cerr << m["hello"] << "\n";
		p->hasState(state);
	}
};*/
int main(int argc, char *argv[]){
        if ( wiringPiSetup() == -1 )
		exit( 1 );
	const int dhtpin = 7;
	const int trig = 4;
	const int echo = 5;
	Ultrasonic* ultra = new Ultrasonic(echo, trig);
	UltraSonicSensorSampleCallback cb1;
	DHT11* dht11 = new DHT11(dhtpin);
	DHT11SampleCallBack cb2;
	pump* p = new pump(0);
	MCP3008* soil_sensor = new MCP3008();
	SoilSensorSampleCallBack cb3;
	const uint8_t soil_channel = 0;
	soil_sensor->set_channel(soil_channel);
	soil_sensor->setCallBack(&cb3);
	dht11->setCallBack(&cb2);
	ultra->setCallBack(&cb1);
	JSONUltraCallback ju(&cb1);
	//JSONpumpCallback jp(&pcb);
	//pumpPOSTCallback jpost(&pcb);
	JSONCGIHandler* fastcgiultra = new JSONCGIHandler(&ju, nullptr, "/tmp/ultrasonic");
	//JSONCGIHandler* fastcgipump = new JSONCGIHandler(&jp, &jpost, "/tmp/pumpsocket");
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
	//delete fastcgipump;
	delete fastcgiultra;
    return 0;
}
