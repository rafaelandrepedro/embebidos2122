#ifndef AIRSENSOR_H
#define AIRSENSOR_H

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <unistd.h>

class Airsensor{
	private:	
		int file;
		uint8_t lastValue;
		bool airsensorState;
	public:
		Airsensor();
		Airsensor(int adapter_nr, int addr);
		~Airsensor();
		bool init(int adapter_nr, int addr);
		bool cread(char buf[10]);
		bool cwrite(char buf[10]);
		float convertTemperatureIS(uint8_t value);
		float convertHumidityIS(uint8_t value);
		bool state();
};

#endif

