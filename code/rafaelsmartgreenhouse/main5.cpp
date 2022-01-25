#include "LDRsensor.h"
#include "WaterTempsensor.h"
#include "airsensor.h"
 
using namespace std;
 
int main(void)
{
    int lightVal = 0;
    int watertemperatureVal = 0;
    LDRsensor light;
    WaterTempsensor water;
    Airsensor airsensor;
	airsensor.init(1, 0x40);
	char buf[10];

    while(1)
    {
        lightVal = light.adcGetValue(0); //read from channel 0
        watertemperatureVal = water.adcGetValue(1); //read from channel 1
        
        buf[0]=0xE3;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	int airTemperature=buf[0];

	buf[0]=0xE5;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	int airHumidity=buf[0];
	
	float Temp_Code = (float)airTemperature;
		airTemperature = ((float)175.72 * (float)Temp_Code / (float)256) - (float)46.85;
		
	float RH_Code = (float)airHumidity;
		airHumidity = ((float)125 * (float)RH_Code / (float)256) + (float)6;


        cout << "Light is: " << lightVal << "%" << endl;
        cout << "Water is: " << watertemperatureVal << "ºC" << endl;
        cout << "Air is: " << airTemperature << "ºC" <<   airHumidity << "RH" << endl;
        sleep(1);
    }

    return 0;
}
