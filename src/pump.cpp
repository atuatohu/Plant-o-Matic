#include "pump.h"
#include "global_variables.h"
#include "soil_callback.cpp"
#include "ultrasonic_callback.cpp"

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
		/*if((soil > 70 || on_off == "on") && level > 5){
			//digitalWrite(relay, 1);
			state = "on";
		}
		if(on_off == "off"){
			digitalWrite(relay, 0);
		}*/
		if(soil > 70 && level > 9 ) digitalWrite(relay, 1);
		else digitalWrite(relay, 0);
}

void pump::run(pump* p){
	p->running = 1;
	//pinMode(p->relay, OUTPUT);
	while(p->running){
		p->onoff();
		/*if(p->pcb){
			p->pcb->hasState(p->state);
		}*/
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
