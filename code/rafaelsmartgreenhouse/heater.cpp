#include "heater.h"

Heater::Heater() {
}


Heater::~Heater(){

}

void Heater::init()
{

    system("insmod /etc/heater.ko");
}

void Heater::turnOn()
{

    system("echo 1 > /dev/heater0");
    this->heaterState = 1;
}

void Heater::turnOff()
{
    system("echo 0 > /dev/heater0");
    this->heaterState = 0;
}

void Heater::actuate(unsigned int dutycycle)
{
    if(dutycycle<=100)
    {
        turnOn();
        usleep(dutycycle*1000);
        turnOff();
        usleep(100000-dutycycle*1000);
    }
}

bool Heater::state()
{
    return heaterState;
}
