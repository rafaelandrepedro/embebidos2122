#include "waterTemperature.h"

WaterTemperature::WaterTemperature(int GPIO);
WaterTemperature::~WaterTemperature();
void WaterTemperature::init(int GPIO);
float WaterTemperature::read();
float WaterTemperature::convertIS(float rawValue);
