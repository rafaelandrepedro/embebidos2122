#ifndef LIGHT_H
#define LIGHT_H

#include <stdlib.h>
#include <string> 

class Light{
	private:	
		bool lightState;
    public:
		Light();
		~Light();
        void init();
		void turnOn();
		void turnOff();
		bool state();
};

#endif
