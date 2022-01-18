#ifndef WATERTEMPSENSOR_H
#define WATERTEMPSENSOR_H

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "spiadc.h"

class WaterTempsensor : public ADCmcp3008{

public:
    WaterTempsensor();
    ~WaterTempsensor();
    virtual int adcGetValue(int channeladc);

};

#endif // WATERTEMPSENSOR_H
