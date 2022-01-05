#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <stdlib.h>
#include <string> 

class WaterPump{
	private:	
		bool pumpState;
		int GPIO;
	public:
		WaterPump();
		WaterPump(int GPIO);
		~WaterPump();
		void init(int GPIO);
		void turnOn();
		void turnOff();
		bool state();
};

#endif
