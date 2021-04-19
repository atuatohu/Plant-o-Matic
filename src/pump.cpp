#include "pump.h"
#include "global_variables.h"
#include "soil_callback.cpp"
#include "ultrasonic_callback.cpp"
#include "treshhold.h"

pump::pump(int relay){
	setPin(relay);
	pinMode(relay, OUTPUT);

}
void pump::setPin(int p){
	relay = p;
}
void pump::setCallback(pumpCallback* p){
		pcb = p;
}
void pump::onoff(){

		if(soil > MAXSOILDRY && level > MINWATERLEVEL ) digitalWrite(relay, 1);
		else digitalWrite(relay, 0);
}

void pump::run(pump* p){
	p->running = 1;
	while(p->running){
		p->onoff();
	}
}
void pump::start(){
	if(pumpThread) throw "called while pumpThread is running";
	pumpThread = new std::thread(run, this);
}
void pump::stop(){
	running = 0;
	if(pumpThread){
		pumpThread->join();
		delete pumpThread;
		pumpThread = NULL;
	}
}
std::string pump::string_state(){
	if (digitalRead(relay) == 1) return "on";
		else return "off";
}
void pump::changeState(std::string state, int cnt){
	on_off = state;
}
