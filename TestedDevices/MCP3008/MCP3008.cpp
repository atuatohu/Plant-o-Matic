#include "MCP3008.h"

MCP3008::MCP3008(){
    wiringPiSPISetup(0, 1000);
}

void MCP3008::set_channel(uint8_t channel){
	adc_channel = channel;
}

void MCP3008::setCallBack(MCP3008callback* cb){
    mcp3008callback = cb;
}

int MCP3008::readData(){
    unsigned int commandout = 0;
    unsigned int adcout = 0;
    commandout = adc_channel & 0x3;  // only 0-7
    commandout |= 0x18;     // start bit + single-ended bit

    uint8_t spibuf[3];

    spibuf[0] = commandout;
    spibuf[1] = 0;
    spibuf[2] = 0;

    wiringPiSPIDataRW(0, spibuf, 3);    

    adcout = ((spibuf[1] << 8) | (spibuf[2])) >> 4;
    return adcout;
}
void MCP3008::run(MCP3008* mcp3008){
    mcp3008->running = 1;
    wiringPiSPISetup(0, 1000);
    while(mcp3008->running){
        int ret = waitForInterrupt(10, 1000);
        if (ret == 0) throw "Interrupt timed out";
        else if(ret == -1) throw "error";
        int value = mcp3008->readData();
        if(mcp3008->mcp3008callback){
            mcp3008->mcp3008callback->hasSample(value);
        }
    }
}
void MCP3008::start(){
    if(adcThread) throw "called while adcThread is running";
    adcThread = new std::thread(run, this);
}
void MCP3008::stop(){
    running = 0;
    if(adcThread){
        adcThread->join();
        delete adcThread;
        adcThread = NULL;
    }
}
/*void MCP3008::read_data(){
	unsigned int commandout = 0;
    //unsigned int adcout = 0;

    commandout = MCP3008::adc_channel & 0x3;  // only 0-7
    commandout |= 0x18;     // start bit + single-ended bit

    uint8_t spibuf[3];

    spibuf[0] = commandout;
    spibuf[1] = 0;
    spibuf[2] = 0;

    wiringPiSPIDataRW(0, spibuf, 3);    

    adcout = ((spibuf[1] << 8) | (spibuf[2])) >> 4;

    //printf("%d\n", adcout);
}*/
