#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "stepmotor.cpp"

int main(){
	StepMotor sm;
	system("echo init");
	sm.init(6,13,19,26,0,1,10);
	system("echo start");
	while(1){
		sm.rotateTo(40);
		sm.rotateTo(0);
	}	
	
}
