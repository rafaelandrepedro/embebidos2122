#include "heater.h"

WaterPump::WaterPump(int GPIO){
	init(GPIO);
}

WaterPump::~WaterPump(){
	
}

void WaterPump::init(int GPIO){
	string command1 = "echo " + std::to_string(GPIO)+ " > /sys/class/gpio/export";
	string command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(GPIO)+ "/direction";
	system(command1);
	system(command2);
	this->GPIO=GPIO;
}

void WaterPump::turnOn(){
	string command = "echo 1 > /sys/class/gpio/" + std::to_string(GPIO)+ "/value";
	system(command);
	pumpState=1;
}

void WaterPump::turnOff(){
	string command = "echo 0 > /sys/class/gpio/" + std::to_string(GPIO)+ "/value";
	system(command);
	pumpState=0;
}

bool WaterPump::state(){
	return pumpState;
}

#endif