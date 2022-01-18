#ifndef LIGHT_H
#define LIGHT_H

#include <stdlib.h>
#include <string> 

class Light{
	private:	
		bool lightState;
		int GPIO;
	public:
		Light();
		Light(int GPIO);
		~Light();
		void init(int GPIO);
		void turnOn();
		void turnOff();
		bool state();
};

#endif