#include "MCP3008.h"
#include "DHT11.h"
#include "json_fastcgi_web_api.h"
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

//Author Bernd Porr

int mainRunning = 1;

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

class SoilSensorSampleCallBack:public MCP3008callback {
public:
	int currentMoist;
	int forcedMoist;
	int forcedCounter = 0;
	long t;
	virtual void hasSample(int s){
		currentMoist = s;
		t = time(NULL);
		printf("soil = %d\n",s);
	}
	void forceMoist(int moist, int step){
		forcedMoist = moist;
		forcedCounter = step;
	}
};

class WaterLevelSensorSampleCallBack:public MCP3008callback {
    virtual void hasSample(int w){
        printf("water level = %d\n",w);
    }
};

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
	void forceTemperature(float temp, int step){
		forceTemp = temp;
		forcedCounter = step;
	}
	void forceHum(float hum, int step){
		forcedHum = hum;
		forcedCounter2 = step;
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
class MCP3008POSTCalback : public JSONCGIHandler::POSTCallback{
public:
	MCP3008POSTCalback(SoilSensorSampleCallBack* s){
		cb = s;
	}
	SoilSensorSampleCallBack* cb;
	virtual void postString(std::string postArg) {
		auto m = JSONCGIHandler::postDecoder(postArg);
		int moist = atof(m["soilmoisture"].c_str());
		int steps = atoi(m["steps"].c_str());
		std::cerr << m["hello"] << "\n";
		cb->forceMoist(moist, steps);
	}
};

class DHT11PostCallbackTemp : public JSONCGIHandler::POSTCallback{
public:
	DHT11SampleCallBack* dht11;
	DHT11PostCallbackTemp(DHT11SampleCallBack* cb){
		dht11 = cb;
	};
	virtual void postString(std::string postArg){
		auto m = JSONCGIHandler::postDecoder(postArg);
		int temperature = atof(m["temperature"].c_str());
		int steps = atoi(m["steps"].c_str());
		std::cerr << m["hello"] << "\n";
		dht11->forceTemperature(temperature, steps);
	}
};
class DHT11PostCallbackHum : public JSONCGIHandler::POSTCallback{
public:
	DHT11SampleCallBack* dht11;
	DHT11PostCallbackHum(DHT11SampleCallBack* cb){
		dht11 = cb;
	};
	virtual void postString(std::string postArg){
		auto m = JSONCGIHandler::postDecoder(postArg);
		int humidity = atof(m["humidity"].c_str());
		int steps = atoi(m["steps"].c_str());
		std::cerr << m["hello"] << "\n";
		dht11->forceHum(humidity, steps);
	}
};

int main(int argc, char *argv[]){
        if ( wiringPiSetup() == -1 )
		exit( 1 );
	int pin = 7;
	DHT11* dht11 = new DHT11(pin);
	DHT11SampleCallBack cb;
	//DHT11SampleCallBack cb2;
	MCP3008* soil_sensor = new MCP3008();
	SoilSensorSampleCallBack print1;
	uint8_t soil_channel = 0;
	soil_sensor->set_channel(soil_channel);
	soil_sensor->setCallBack(&print1);
	dht11->setCallBack(&cb);
	JSONCGIMCP3008Callback jc(&print1);
	MCP3008POSTCalback mcppost(&print1);
	JSONCGIHandler* fastcgi = new JSONCGIHandler(&jc, &mcppost, "/tmp/soilsocket");
	JSONDHT11CallbackTemp jcdht(&cb);
	DHT11PostCallbackTemp tempPost(&cb);
	JSONCGIHandler* fastcgidht = new JSONCGIHandler(&jcdht, &tempPost, "/tmp/tempsocket");
	JSONDHT11CallbackHum humdht(&cb);
	DHT11PostCallbackHum humPost(&cb);
	JSONCGIHandler* fastcgihum = new JSONCGIHandler(&humdht, &humPost, "/tmp/humsocket");
	soil_sensor->start();
	dht11->start();
	setHUPHandler();

	fprintf(stderr,"'%s' up and running.\n",argv[0]);

	while (mainRunning) sleep(1);

	fprintf(stderr,"'%s' shutting down.\n",argv[0]);
	dht11->stop();
	soil_sensor->stop();
	delete soil_sensor;
	delete dht11;
	delete fastcgidht;
	delete fastcgihum;
	delete fastcgi;
    return 0;
}
