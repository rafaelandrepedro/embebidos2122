#include "waterpump.h"

WaterPump::WaterPump(){
}

WaterPump::WaterPump(int GPIO){
	init(GPIO);
}

WaterPump::~WaterPump(){
	
}

void WaterPump::init(int GPIO){
	std::string command1 = "echo " + std::to_string(GPIO)+ " > /sys/class/gpio/export";
	std::string command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(GPIO)+ "/direction";
	system(command1.c_str());
	system(command2.c_str());
	this->GPIO=GPIO;
}

void WaterPump::turnOn(){
	std::string command = "echo 1 > /sys/class/gpio/gpio" + std::to_string(GPIO)+ "/value";
	system(command.c_str());
	pumpState=1;
}

void WaterPump::turnOff(){
	std::string command = "echo 0 > /sys/class/gpio/gpio" + std::to_string(GPIO)+ "/value";
	system(command.c_str());
	pumpState=0;
}

bool WaterPump::state(){
	return pumpState;
}

