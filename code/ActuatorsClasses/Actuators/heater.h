#ifndef HEATER_H
#define HEATER_H

#include <stdlib.h>
#include <string> 
#include <unistd.h>

class Heater{
	private:	
		bool heaterState;
	public:
		Heater();
		~Heater();
        void init();
		void turnOn();
		void turnOff();
        void actuate(unsigned int dutycycle);
		bool state();
};

#endif
