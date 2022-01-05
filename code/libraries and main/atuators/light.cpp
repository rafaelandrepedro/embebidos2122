#include "light.h"

Light::Light(int GPIO){
	init(GPIO);
}

Light::~Light(){
	
}

void Light::init(int GPIO){
	string command1 = "echo " + std::to_string(GPIO)+ " > /sys/class/gpio/export";
	string command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(GPIO)+ "/direction";
	system(command1);
	system(command2);
	this->GPIO=GPIO;
}

void Light::turnOn(){
	string command = "echo 1 > /sys/class/gpio/" + std::to_string(GPIO)+ "/value";
	system(command);
	lightState=1;
}

void Light::turnOff(){
	string command = "echo 0 > /sys/class/gpio/" + std::to_string(GPIO)+ "/value";
	system(command);
	lightState=0;
}

bool Light::state(){
	return lightState;
}

#endif