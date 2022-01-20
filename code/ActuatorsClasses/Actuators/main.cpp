#include <iostream>
#include "light.h"
#include "stepmotor.h"
using namespace std;

int main()
{
    Light l;
    l.init();
    StepMotor M(19,13,6,26,0,10000,0);
    M.rotateTo(512);
    l.turnOn();
    cout << "Hello World!" << endl;
    return 0;
}
