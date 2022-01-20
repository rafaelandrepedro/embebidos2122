#include "light.h"

Light::Light() {
}

Light::Light(int GPIO){
	init(GPIO);
}

Light::~Light(){
	
}

void Light::init(int GPIO){
	std::string command1 = "echo " + std::to_string(GPIO) + " > /sys/class/gpio/export";
	std::string command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(GPIO) + "/direction";
	system(command1.c_str());
	system(command2.c_str());
	this->GPIO=GPIO;
}

void Light::turnOn(){
	std::string command = "echo 1 > /sys/class/gpio/gpio" + std::to_string(GPIO) + "/value";
	system(command.c_str());
	lightState=1;
}

void Light::turnOff(){
	std::string command = "echo 0 > /sys/class/gpio/gpio" + std::to_string(GPIO) + "/value";
	system(command.c_str());
	lightState=0;
}

bool Light::state(){
	return lightState;
}

