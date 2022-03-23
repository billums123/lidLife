/*Functions for using the strain gauge measuring the force to raise the lid*/

#ifndef StrainGauge_h
#define StrainGauge_h
#include "HX711.h"    
#include "Arduino.h"

// Place your main header code here.

class StrainGauge
{
    private:
    int _dout_pin;
    int _clk_pin;
    float _calibration_factor;
    String _strainGaugeName;
    HX711 _strainGauge;
public:
    StrainGauge(int strainGaugeDout, int strainGaugeClk, float strainGaugeCalibF, String _strainGaugeName);
    bool Setup();
    float MeasureForce();
    double CalbrateScale();
};

#endif // StrainGauge_h