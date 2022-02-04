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
        usleep(dutycycle*10000);
        turnOff();
        heaterState=1;
        usleep(1000000-dutycycle*10000);
        heaterState=0;
    }
}

bool Heater::state()
{
    return heaterState;
}
