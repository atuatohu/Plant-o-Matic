#include "DHT11.h"
#include <wiringPi.h>

void DHT11::set_pin(int pin){
	DHT11::DHTPIN = pin;
}

void DHT11::read_dht_data(){
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 
	int dht11_dat[5] = { 0, 0, 0, 0, 0 };
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode(DHT11::DHTPIN, OUTPUT );
	digitalWrite(DHT11::DHTPIN, LOW );
	delay( 18 );
	digitalWrite(DHT11::DHTPIN, HIGH );
	delayMicroseconds( 40 );
	pinMode(DHT11::DHTPIN, INPUT );
 
	for ( i = 0; i < DHT11::MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead(DHT11::DHTPIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead(DHT11::DHTPIN );
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) )
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 55 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
	}else  {
		printf( "Data not good, skip\n" );
	}
	
	}
