#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <stdlib.h>
#include <string>

class WaterPump{
    private:
        bool waterpumpState;
    public:
        WaterPump();
        ~WaterPump();
        void init();
        void turnOn();
        void turnOff();
        bool state();
};

#endif
