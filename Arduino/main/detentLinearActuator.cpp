#include "Arduino.h"
#include "DetentLinearActuator.h"

DetentLinearActuator::DetentLinearActuator(int linActDir1, int linActDir2, int linActPinPWM)
{
    _linActDir1 = linActDir1;
    _linActDir2 = linActDir2;
    _linActPinPWM = linActPinPWM;
}

bool DetentLinearActuator::CheckIfDetentsAreUsed(String detentUseStatus)
{
    bool detentsInUse;
    if ((detentUseStatus == "y") || (detentUseStatus == "Y") || (detentUseStatus == "yes") || (detentUseStatus == "Yes"))
    {
        return (detentsInUse = true);
    }
    else if ((detentUseStatus == "n") || (detentUseStatus == "N") || (detentUseStatus == "no") || (detentUseStatus == "No"))
    {
        return (detentsInUse = false);
    }
    else
    {
        Serial.println("Invalid character input, please try again.");
        
    }
}

void DetentLinearActuator::Setup()
{
    pinMode(_linActDir1, OUTPUT);
    pinMode(_linActDir2, OUTPUT);
    pinMode(_linActPinPWM, OUTPUT);
    digitalWrite(_linActPinPWM, LOW);
}

void DetentLinearActuator::Extend()
{
    digitalWrite(_linActDir1, LOW);
    digitalWrite(_linActDir2, HIGH);
    digitalWrite(_linActPinPWM, HIGH);
    // delay(extensionTime);
    // digitalWrite(_linActPinPWM, LOW);
}
void DetentLinearActuator::Retract()
{
    digitalWrite(_linActDir1, HIGH);
    digitalWrite(_linActDir2, LOW);
    digitalWrite(_linActPinPWM, HIGH);
    // delay(retractionTime);
    // digitalWrite(_linActPinPWM, LOW);
}
void DetentLinearActuator::Stop()
{
    digitalWrite(_linActPinPWM, LOW);
    // delay(retractionTime);
    // digitalWrite(_linActPinPWM, LOW);
}
