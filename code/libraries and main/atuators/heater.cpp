#include "heater.h"

Heater::Heater(int GPIO){
	init(GPIO);
}

Heater::~Heater(){
	
}

void Heater::init(int GPIO){
	string command1 = "echo " + std::to_string(GPIO)+ " > /sys/class/gpio/export";
	string command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(GPIO)+ "/direction";
	system(command1);
	system(command2);
}

void Heater::turnOn(){
	system("echo 1 > /sys/class/gpio/gpio17/value");
	heaterState=1;
}

void Heater::turnOff(){
	system("echo 0 > /sys/class/gpio/gpio17/value");
	heaterState=0;
}

bool Heater::state(){
	return heaterState;
}

#endif