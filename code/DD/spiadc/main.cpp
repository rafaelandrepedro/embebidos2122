#include "LDRsensor.h"
#include "WaterTempsensor.h"
 
using namespace std;
 
int main(void)
{
    int lightVal = 0;
    int watertemperatureVal = 0;
    LDRsensor light;
    WaterTempsensor water;


    while(1)
    {
        lightVal = light.adcGetValue(0); //read from channel 0
        watertemperatureVal = water.adcGetValue(1); //read from channel 1


        cout << "Light is: " << lightVal << "%" << endl;
        cout << "Water is: " << watertemperatureVal << "ºC" << endl;
        sleep(1);
    }

    return 0;
}
