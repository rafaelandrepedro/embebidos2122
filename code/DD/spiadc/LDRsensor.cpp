#include "LDRsensor.h"


LDRsensor::LDRsensor(){}

LDRsensor::~LDRsensor(){}

int LDRsensor::adcGetValue(int channeladc)
{
    int value = 0;
    unsigned char data[3];

    data[0] = 1;  //start bit
    data[1] = 0b10000000 |( ((channeladc & 7) << 4)); //(SINGLE/DIF = 1)
    data[2] = 0; // third byte transmitted don't care

    this->spiWriteRead(data, sizeof(data));

    value = (data[1]<< 8) & 0b1100000000;
    value |=  (data[2] & 0xff);

    //value = value/1024;
    value = value*100 / 1024;
    return value;
}

