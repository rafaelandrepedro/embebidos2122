#include "spiadc.h"
using namespace std;


//
//Opens spidev path and configures spi mode, bits and speed
int ADCmcp3008::spiOpen(std::string devspi)
{
    int statusVal = -1;
    this->spifd = open(devspi.c_str(), O_RDWR);
    if(this->spifd < 0)
    {
        perror("could not open SPI device");
        exit(1);
    }
 
    statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    if(statusVal < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }
 
    statusVal = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
    if(statusVal < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }
 
    statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }
 
    statusVal = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }
 
    statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));    
    if(statusVal < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }
 
    statusVal = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return statusVal;
}



//
//Closes spidev interface
int ADCmcp3008::spiClose(){
    int statusVal = -1;
    statusVal = close(this->spifd);
    if(statusVal < 0)
    {
      perror("Could not close SPI device");
      exit(1);
    }
    return statusVal;
}
 


//
//writes data on buffer "data" and receives response on the same buffer
int ADCmcp3008::spiWriteRead(unsigned char *data, int length)
{
  struct spi_ioc_transfer spi[length];
  int i = 0;
  int retVal = -1; 
  bzero(spi, sizeof spi); // clear ioctl struct
 
// one spi transfer for each byte
 
  for (i = 0 ; i < length ; i++)
  {
    spi[i].tx_buf        = (unsigned long)(data + i); // transmit "data" buffer
    spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data" buffer
    spi[i].len           = sizeof(*(data + i)) ;
    spi[i].delay_usecs   = 0 ;
    spi[i].speed_hz      = this->speed ;
    spi[i].bits_per_word = this->bitsPerWord ;
    spi[i].cs_change = 0;
  }
 
  if(ioctl(this->spifd, SPI_IOC_MESSAGE(length), &spi) < 0)
  {
    perror("Problem transmitting spi data..ioctl");
    exit(1);
  }
 
  return retVal;

}


//
//virtual function overriden by sensor classes
int ADCmcp3008::adcGetValue(int channeladc)
{
    return -1;
}

//
//Default contructor
ADCmcp3008::ADCmcp3008()
{
   this->mode = SPI_MODE_0 ;
   this->bitsPerWord = 8;
   this->speed = 1000000;
   this->spifd = -1;
 
   this->spiOpen(std::string("/dev/spidev1.0"));
 
}
 
//
//parameter constructor receives spidev path, mode, speed and bits
ADCmcp3008::ADCmcp3008(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord)
{
    this->mode = spiMode ;
    this->bitsPerWord = spibitsPerWord;
    this->speed = spiSpeed;
    this->spifd = -1; 
    this->spiOpen(devspi);
}
 
//
//Destructor~
ADCmcp3008::~ADCmcp3008()
{
    this->spiClose();
}
