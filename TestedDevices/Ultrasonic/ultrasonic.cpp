#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>



#define TRIG 4
#define ECHO 5

int getDistanceCM() {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);

        //Wait for echo start
        while(digitalRead(ECHO) == LOW);

        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;

        //Get distance in cm
        int distance = travelTime / 58;

        return distance;
		
}

int main() {
	wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, LOW); //TRIG pin must start LOW
    delay(30);
	while (1)
	{
		int dist=getDistanceCM();
		printf("Distance: %dcm\n", dist);
		delay(1000);
	
	}
	return 0;
}

