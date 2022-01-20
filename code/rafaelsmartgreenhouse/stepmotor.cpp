#include "stepmotor.h"

StepMotor::StepMotor(){
}

StepMotor::StepMotor(int A1, int A2, int B1, int B2, int motorStep, unsigned int stepDelay, float angleStep){
	init(A1, A2, B1, B2, motorStep, stepDelay, angleStep);
}

StepMotor::~StepMotor(){

}

void StepMotor::init(int A1, int A2, int B1, int B2, int motorStep, unsigned int stepDelay, float angleStep){
	this->A1=A1;
	this->A2=A2;
	this->B1=B1;
	this->B2=B2;
	
	std::string command1;
	std::string command2;
	
	command1 = "echo " + std::to_string(A1)+ " > /sys/class/gpio/export";
	command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(A1)+ "/direction";
	system(command1.c_str());
	system(command2.c_str());
	
	command1 = "echo " + std::to_string(A2)+ " > /sys/class/gpio/export";
	command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(B1)+ "/direction";
	system(command1.c_str());
	system(command2.c_str());
	
	command1 = "echo " + std::to_string(B1)+ " > /sys/class/gpio/export";
	command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(A2)+ "/direction";
	system(command1.c_str());
	system(command2.c_str());
	
	command1 = "echo " + std::to_string(B2)+ " > /sys/class/gpio/export";
	command2 = "echo out > /sys/class/gpio/gpio" + std::to_string(B2)+ "/direction";
	system(command1.c_str());
	system(command2.c_str());
	
	this->motorStep=motorStep;
	this->motorStepAngle=angleStep;
	this->stepDelay=stepDelay;
	motorState=0b0011;
}

bool StepMotor::state(){
	if(motorStep==0)
		return 0;
	return 1;
}

void StepMotor::step(bool direction){
	usleep(stepDelay);
	if(direction)
		switch(motorState){
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
		switch(motorState){
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
		
	std::string command;
	
	command = "echo " + std::to_string((bool)(motorState&(1<<0))) + " > /sys/class/gpio/gpio" + std::to_string(A1) + "/value";
	system(command.c_str());
	command = "echo " + std::to_string((bool)(motorState&(1<<1))) + " > /sys/class/gpio/gpio" + std::to_string(A2) + "/value";
	system(command.c_str());
	command = "echo " + std::to_string((bool)(motorState&(1<<2))) + " > /sys/class/gpio/gpio" + std::to_string(B1) + "/value";
	system(command.c_str());	
	command = "echo " + std::to_string((bool)(motorState&(1<<3))) + " > /sys/class/gpio/gpio" + std::to_string(B2) + "/value";
	system(command.c_str());
	command = "echo " + std::to_string((bool)(motorState&(1<<3))) + " " + std::to_string(B2) + " " + std::to_string(motorState);
	system(command.c_str());
}

void StepMotor::rotate(int steps){
	if(steps>0)
		for(int i=0;i<steps;i++){
			step(1);
			motorStep++;
		}
	else
		for(int i=0;i<-steps;i++){
			step(0);
			motorStep--;
		}
}

void StepMotor::rotateTo(int absoluteStep){
	int steps=absoluteStep-motorStep;
	rotate(steps);
}

int StepMotor::getAngle(){
	return motorStep*motorStepAngle;
}

