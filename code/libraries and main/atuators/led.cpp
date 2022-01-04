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
}

void Light::turnOn(){
	system("echo 1 > /sys/class/gpio/gpio17/value");
	lightState=1;
}

void Light::turnOff(){
	system("echo 0 > /sys/class/gpio/gpio17/value");
	lightState=0;
}

bool Light::state(){
	return lightState;
}

#endif