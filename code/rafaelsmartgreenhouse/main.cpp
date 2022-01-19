#include <iostream>
#include "buffer.h"
#include "airsensor.h"


int main()
{
    Airsensor sensor;
    sensor.init(1, 0x40);
    std::cout << "Hello World!" << std::endl;
    char buf[10];
    while(1){
        buf[0]=0xE5;
        sensor.cwrite(buf);
        while(!sensor.cread(buf)){}
        std::cout << (int)buf[0] << std::endl;
        buf[0]=0xE3;
        sensor.cwrite(buf);
        while(!sensor.cread(buf)){}
        std::cout << (int)buf[0] << std::endl;
        sleep(1);
    }
    return 0;
}
