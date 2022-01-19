#ifndef LDRSENSOR_H
#define LDRSENSOR_H

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

class LDRsensor : public ADCmcp3008{

public:
    LDRsensor();
    ~LDRsensor();
    virtual int adcGetValue(int channeladc);

};

#endif // LDRSENSOR_H
