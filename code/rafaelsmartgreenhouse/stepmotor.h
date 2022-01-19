#ifndef STEPMOTOR_H
#define STEPMOTOR_H
#include <stdlib.h>
#include <string> 
#include <iostream>
#include <unistd.h>

class StepMotor{
	private:
		float motorStepAngle;
		int motorStep;
		int stepDelay;
		int motorState;
		int A1, A2, B1, B2;
	public:
		StepMotor();
		StepMotor(int A1, int A2, int B1, int B2, int motorStep, unsigned int stepDelay, float angleStep);
		~StepMotor();
		void init(int A1, int A2, int B1, int B2, int motorStep, unsigned int stepDelay, float angleStep);
		bool state();
		void step(bool direction);
		void rotate(int angle);
		void rotateTo(int absoluteAngle);
		int getAngle();
};

#endif
