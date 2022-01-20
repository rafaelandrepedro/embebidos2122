#include "waterpump.h"

WaterPump::WaterPump() {
}


WaterPump::~WaterPump(){

}

void WaterPump::init()
{

    system("insmod /etc/waterpump.ko");
}

void WaterPump::turnOn()
{

    system("echo 1 > /dev/waterpump0");
    this->waterpumpState = 1;
}

void WaterPump::turnOff()
{
    system("echo 0 > /dev/light0");
    this->waterpumpState = 0;
}

bool WaterPump::state()
{
    return waterpumpState;
}

