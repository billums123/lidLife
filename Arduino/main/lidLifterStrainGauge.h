/*Functions for using the strain gauge measuring the force to raise the lid*/

#ifndef lidLifterStrainGauge_h    
#include "HX711.h"
#define lidLifterStrainGauge_h        

// Place your main header code here.

class lidLifterStrainGaugeClass    
{
    public:
    lidLifterStrainGaugeClass();
    void setup(int lidLiftStrainGauge_DOUT_pin, int lidLiftStrainGauge_SCK_pin);

};

extern lidLifterStrainGaugeClass lidLifterStrainGauge;
#endif // lidLifterStrainGauge_h        