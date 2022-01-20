#include "airsensor.h"


Airsensor::Airsensor(){
	system("echo config");
	system("modprobe i2c-bcm2835");
	system("modprobe i2c-dev");
}

Airsensor::Airsensor(int adapter_nr, int addr){
	system("echo config");
	system("modprobe i2c-bcm2835");
	system("modprobe i2c-dev");
	init(adapter_nr, addr);
}

Airsensor::~Airsensor(){}
   
bool Airsensor::init(int adapter_nr=1, int addr=0x40){
  if (airsensorState==1)
        return 0;
  char filename[20];
  
  airsensorState=0;

  snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
  file = open(filename, O_RDWR);
  if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    printf("cant run open");
    return 0;
  }

  if (ioctl(file, I2C_SLAVE, addr) < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    printf("cant run ioctl");
    return 0;
  }
  __u8 reg = 0x10; /* Device register to access */
  __s32 res;
  airsensorState=1;
  return 1;
}

bool Airsensor::cwrite(char buf[10]){
  if (write(file, buf, 3) != 3) {
    /* ERROR HANDLING: i2c transaction failed */
    return 0;
  }
  return 1;
}

bool Airsensor::cread(char buf[10]){
  /* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
  if (read(file, buf, 1) != 1) {
    /* ERROR HANDLING: i2c transaction failed */
    return 0;
  } else {
    /* buf[0] contains the read byte */
    lastValue=(uint8_t)buf[0];
    return 1;
  }
}

float Airsensor::convertTemperatureIS(uint8_t value){
	float Temp_Code = value;
	return (175.72 * Temp_Code / 65536) + 46.85;
}

float Airsensor::convertHumidityIS(uint8_t value){
	float RH_Code = value;
	return (125 * RH_Code / 65536) + 6;
}

bool Airsensor::state(){return airsensorState;}

