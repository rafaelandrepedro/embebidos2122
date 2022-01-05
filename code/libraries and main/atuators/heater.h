#ifndef HEATER_H
#define HEATER_H

#include <stdlib.h>
#include <string> 

class Heater{
	private:	
		bool heaterState;
		int GPIO;
	public:
		Heater();
		Heater(int GPIO);
		~Heater();
		void init(int GPIO);
		void turnOn();
		void turnOff();
		bool state();
};

#endif