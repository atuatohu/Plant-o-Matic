#include "MCP3008.h"

void MCP3008::set_channel(uint8_t channel){
	MCP3008::adc_channel = channel;
}

void MCP3008::read_data(){
	unsigned int commandout = 0;
    unsigned int adcout = 0;

    commandout = MCP3008::adc_channel & 0x3;  // only 0-7
    commandout |= 0x18;     // start bit + single-ended bit

    uint8_t spibuf[3];

    spibuf[0] = commandout;
    spibuf[1] = 0;
    spibuf[2] = 0;

    wiringPiSPIDataRW(0, spibuf, 3);    

    adcout = ((spibuf[1] << 8) | (spibuf[2])) >> 4;

    printf("%d\n", adcout);
}
