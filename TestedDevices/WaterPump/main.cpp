#include <stdio.h>
#include <wiringPi.h>

// REL pin - Relay (wiringPi pin 0 / GPIO 17)

#define REL     0

int main (void)
{
  wiringPiSetup () ;
  pinMode (LED, OUTPUT) ;

  for (;;)
  {
    // Relay On
    digitalWrite (REL, 1) ;
    delay (1500) ;
    // Relay Off         
    digitalWrite (REL, 0) ;
    delay (1500) ;
  }
  return 0 ;
}
