#include "Arduino.h"
#include "HX711.h"
#include "lidLifterStrainGauge.h"
HX711 lidLifterScale;

lidLifterStrainGaugeClass::lidLifterStrainGaugeClass()
{
}

void lidLifterStrainGaugeClass::setup(int lidLiftStrainGauge_DOUT_pin, int lidLiftStrainGauge_SCK_pin){
lidLifterScale.begin(lidLiftStrainGauge_DOUT_pin, lidLiftStrainGauge_SCK_pin);
}  



lidLifterStrainGaugeClass lidLifterStrainGauge = lidLifterStrainGaugeClass();