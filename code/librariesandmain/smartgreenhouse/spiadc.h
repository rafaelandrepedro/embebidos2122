#ifndef SPIADC_H
#define SPIADC_H

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
 
 
class ADCmcp3008{
 
public:
    ADCmcp3008();
    ADCmcp3008(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord);
    ~ADCmcp3008();
    int spiWriteRead(unsigned char *data, int length);
    int adcGetValue(int channeladc);
     
private:
    unsigned char mode;
    unsigned char bitsPerWord;
    unsigned int speed;
    int spifd;
     
    int spiOpen(std::string devspi);
    int spiClose();
     
};
 
#endif
