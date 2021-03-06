#include "DHT11.h"
#include <wiringPi.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
	DHT11* dht11 = new DHT11();
	int pin = 7;
	dht11->set_pin(pin);
	if ( wiringPiSetup() == -1 )
		exit( 1 );
 
	while ( 1 )
	{
		dht11->read_dht_data();
		delay( 2000 ); 
	}
	return 0;
}
