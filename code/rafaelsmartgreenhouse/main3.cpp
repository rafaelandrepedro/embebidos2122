#include <iostream>
#include "stepmotor.h"
#include <unistd.h>

int main()
{
	StepMotor stepmotor;
	stepmotor.init(19, 6, 13, 26, 0, 10000, 0);
    std::cout << "Hello World!" << std::endl;
    while(1){
	stepmotor.rotate(64);
	stepmotor.rotate(-64);
    }
    return 0;
}
