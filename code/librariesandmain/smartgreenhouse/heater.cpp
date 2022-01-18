#include "heater.h"

Heater::Heater() {
}

Heater::Heater(int GPIO){
	init(GPIO);
}

Heater::~Heater(){
	
}

void Heater::init(int GPIO){
	std::string command1 = "echo " + std::to_string(GPIO) + " > /sys/class/gpio/export";
	std::string command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(GPIO) + "/direction";
	system(command1.c_str());
	system(command2.c_str());
	this->GPIO=GPIO;
}

void Heater::turnOn(){
	std::string command = "echo 1 > /sys/class/gpio/gpio" + std::to_string(GPIO) + "/value";
	system(command.c_str());
	heaterState=1;
}

void Heater::turnOff(){
	std::string command = "echo 0 > /sys/class/gpio/gpio" + std::to_string(GPIO) + "/value";
	system(command.c_str());
	heaterState=0;
}

void Heater::set(unsigned int dutycycle){
	if(dutycycle<=100){
		std::string command = "echo " + std::to_string(dutycycle);
		system(command.c_str());
		turnOn();
		usleep(dutycycle*1000);
		turnOff();
		usleep(100000-dutycycle*1000);
	}
}

bool Heater::state(){
	return heaterState;
}

