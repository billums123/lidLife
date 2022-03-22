#include "Arduino.h"
#include "detentLinearActuator.h"

detentLinearActuatorClass::detentLinearActuatorClass()
{
}

bool detentLinearActuatorClass::checkIfDetentsAreUsed(String detentUseStatus)
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
        return;
    }
}

void detentLinearActuatorClass::setup(int linActDir1, int linActDir2, int linActPinPWM)
{
    pinMode(linActDir1, OUTPUT);
    pinMode(linActDir2, OUTPUT);
    pinMode(linActPinPWM, OUTPUT);
}

void detentLinearActuatorClass::extend(int linActDir1, int linActDir2, int linActPinPWM, int extensionTime)
{
    digitalWrite(linActDir1, LOW);
    digitalWrite(linActDir2,HIGH);
    digitalWrite(linActPinPWM, HIGH);
    delay(extensionTime);
    digitalWrite(linActPinPWM, LOW);
}
void detentLinearActuatorClass::retract(int linActDir1, int linActDir2, int linActPinPWM, int rectractionTime)
{
    digitalWrite(linActDir1, HIGH);
    digitalWrite(linActDir2,LOW);
    digitalWrite(linActPinPWM, HIGH);
    delay(rectractionTime);
    digitalWrite(linActPinPWM, LOW);
}

detentLinearActuatorClass detentLinearActuator = detentLinearActuatorClass();