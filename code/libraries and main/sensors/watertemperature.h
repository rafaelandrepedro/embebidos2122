#ifndef WATERTEMPERATURE_H
#define WATERTEMPERATURE_H

#include <stdlib.h>
#include <string> 

class WaterTemperature{
	private:	
		bool lastValue;
		int GPIO;
	public:
		WaterTemperature(int GPIO);
		~WaterTemperature();
		void init(int GPIO);
		float read();
		float convertIS(float rawValue);
};

#endif