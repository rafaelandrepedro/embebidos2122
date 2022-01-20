#include "stepmotor.h"

#include <cstdio>

StepMotor::StepMotor(){
}

StepMotor::StepMotor(int A1, int A2, int B1, int B2, int motorStep, unsigned int stepDelay, float angleStep)
{
	init(A1, A2, B1, B2, motorStep, stepDelay, angleStep);
}

StepMotor::~StepMotor()
{

}

void StepMotor::init(int A1, int A2, int B1, int B2, int motorStep, unsigned int stepDelay, float angleStep)
{
	this->A1=A1;
	this->A2=A2;
	this->B1=B1;
	this->B2=B2;
	
	std::string command1;
	std::string command2;
	
    this->motorStep = motorStep;
    this->motorStepAngle = angleStep;
    this->stepDelay = stepDelay;
    motorState = 0b0011;

    system("insmod /etc/stepmotor.ko");
}

bool StepMotor::state()
{
	if(motorStep==0)
		return 0;
	return 1;
}

void StepMotor::step(bool direction)
{
	usleep(stepDelay);
	if(direction)
        switch(motorState)
        {
		case 0b0011:
			motorState=0b0110;
			break;
		case 0b0110:
			motorState=0b1100;
			break;
		case 0b1100:
			motorState=0b1001;
			break;
		case 0b1001:
			motorState=0b0011;
			break;
		}
	else
        switch(motorState)
        {
		case 0b1100:
			motorState=0b0110;
			break;
		case 0b1001:
			motorState=0b1100;
			break;
		case 0b0011:
			motorState=0b1001;
			break;
		case 0b0110:
			motorState=0b0011;
			break;
		}

    char command1[25];
    sprintf(command1, "echo %X > /dev/stepmotor0", motorState);
    system(command1);

}

void StepMotor::rotate(int steps){
	if(steps>0)
		for(int i=0;i<steps;i++)
			step(1);
	else
		for(int i=0;i<-steps;i++)
			step(0);
}

void StepMotor::rotateTo(int absoluteStep){
	int steps=absoluteStep-motorStep;
	rotate(steps);
}

int StepMotor::getAngle(){
	return motorStep*motorStepAngle;
}

