/*Functions for using the strain gauge measuring the force to raise the lid*/

#ifndef IRSensors_h
#define IRSensors_h
#include "Arduino.h"

// Place your main header code here.

class IRSensors
{
private:
    int _bottomIRSensor;
    int _topIRSensor;
    unsigned long _startOfCycleTime;
    unsigned long _endOfCycleTime;
    ;

public:
    IRSensors(int bottomIRSensor, int topIRSensor);
    void Setup();
    bool SenseClosed();
    bool SenseOpen();
    void StartTimer();
    unsigned long StopTimer();
};

#endif // IRSensors_h