#include <iostream>
#include "stepmotor.h"
#include <unistd.h>

int main()
{
	StepMotor stepmotor;
	stepmotor.init(19, 13, 6, 26, 0, 1000, 0);
    std::cout << "Hello World!" << std::endl;
    while(1){
	stepmotor.rotate(512);
	
	stepmotor.rotate(-512);
	std::cout << "-" << std::endl;
    }
    return 0;
}
