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
 
void mcp3008_read(uint8_t);

// read a channel
void mcp3008_read(uint8_t adcnum)
{ 
    unsigned int commandout = 0;
    unsigned int adcout = 0;

    commandout = adcnum & 0x3;  // only 0-7
    commandout |= 0x18;     // start bit + single-ended bit

    uint8_t spibuf[3];

    spibuf[0] = commandout;
    spibuf[1] = 0;
    spibuf[2] = 0;

    wiringPiSPIDataRW(0, spibuf, 3);    

    adcout = ((spibuf[1] << 8) | (spibuf[2])) >> 4;

    printf("%d\n", adcout);
    
} 
int main(void)
{
    wiringPiSPISetup(0, 1000);
    delay(50);
    for (;;){
        printf("soil ");
        mcp3008_read(0);
        printf("water level ");
        mcp3008_read(1);
        delay(50);
    }
    
    return 0;
}


