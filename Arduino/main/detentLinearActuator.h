/*Functions for user to control the linear actuator used to measure closing force of lid*/

#ifndef DetentLinearActuator_h
#define DetentLinearActuator_h
#include "Arduino.h"
// Place your main header code here.

class DetentLinearActuator
{
private:
    int _linActDir1;
    int _linActDir2;
    int _linActPinPWM;

public:
    DetentLinearActuator(int linActDir1, int linActDir2, int _linActPinPWM);
    bool CheckIfDetentsAreUsed(String detentUseStatus);
    void Setup();
    void Extend();
    void Retract();
    void Stop();
};

#endif // DetentLinearActuator_h