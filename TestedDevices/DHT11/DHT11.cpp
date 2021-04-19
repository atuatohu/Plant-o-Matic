#include "DHT11.h"
#include <wiringPi.h>

DHT11::DHT11(int pin){
	wiringPiSetup();
	DHT11::set_pin(pin);
}

void DHT11::set_pin(int pin){
	DHTPIN = pin;
}

bool DHT11::read_dht_data(){
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	//pull pin down for 18 milliseconds
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay( 18 );
	
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);
	
	//prepare to read the pin
	pinMode(DHTPIN, INPUT);
	
	//detect change and read data
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead(DHTPIN) == laststate )
		{
			counter++;
			delayMicroseconds(1);
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead(DHTPIN);
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) ) //ignore first three transitions 
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 30 )
				dht11_dat[j / 8] |= 1; //combine bits into bytes
			j++;
		}
	}
		//check sensor was read 40 bits and verify checksum in the last byte
		if ( (j >= 40) &&
			 (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
		{
			return true;
		}else{
			return false;
		}	
}
void DHT11::setCallBack(DHT11callback* cb){
	dht11callback = cb;
}
void DHT11::run(DHT11* dht11){
	dht11->running = 1;
	while(dht11->running){
		if(dht11->read_dht_data()){
			if(dht11->dht11callback){
				dht11->dht11callback->hasSample(dht11->dht11_dat);
			}
		}
	}
}
void DHT11::start(){
	if(dhtThread) throw "called while dhtThread is running";
	dhtThread = new std::thread(run, this);
}
void DHT11::stop(){
	running = 0;
	if(dhtThread){
		dhtThread->join();
		delete dhtThread;
		dhtThread = NULL;
	}
}
