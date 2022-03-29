#include "IRSensors.h"
unsigned long currentTime;
IRSensors::IRSensors(int bottomIRSensor, int topIRSensor)
{
    _bottomIRSensor = bottomIRSensor;
    _topIRSensor = topIRSensor;
}
void IRSensors::Setup()
{
    pinMode(_bottomIRSensor, INPUT);
    pinMode(_topIRSensor, INPUT);
}
bool IRSensors::SenseClosed()
{
    bool senseClosed = digitalRead(_bottomIRSensor);
    return (senseClosed);
}
bool IRSensors::SenseOpen()
{
    bool senseOpen = digitalRead(_topIRSensor);
    return (senseOpen);
}
void IRSensors::StartTimer()
{
    _startOfCycleTime = millis();
}
unsigned long IRSensors::StopTimer()
{
    currentTime = millis();
    _endOfCycleTime = currentTime - _startOfCycleTime;
    return (_endOfCycleTime);
}
