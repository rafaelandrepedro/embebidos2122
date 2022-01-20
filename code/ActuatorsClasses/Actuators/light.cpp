#include "light.h"

Light::Light() {
}


Light::~Light(){
	
}

void Light::init()
{

    system("insmod /etc/light.ko");
}

void Light::turnOn()
{

    system("echo 1 > /dev/light0");
    this->lightState = 1;
}

void Light::turnOff()
{
    system("echo 0 > /dev/light0");
    this->lightState = 0;
}

bool Light::state()
{
	return lightState;
}
