#include <iostream>
#include "buffer.h"
#include "airsensor.h"


int main()
{
    Airsensor sensor;
    //sensor.init(1, 0x40);
    std::cout << "Hello World!" << std::endl;
    char buf[10];
    buf[0]=0xE5;
    //sensor.cwrite(buf);
    //sensor.cread(buf);
    //std::cout << buf[0] << std::endl;
    return 0;
}
