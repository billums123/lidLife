/*Functions for using the Stepper Motor that raises the lid*/

#ifndef LidStepperMotor_h
#define LidStepperMotor_h

#include <Stepper.h>
#include <Math.h>
#include "Arduino.h"
#include <AccelStepper.h>
class LidStepperMotor : public AccelStepper
{
private:
    int _stepperMotorDirPin;
    int _stepperMotorPulPin;
    int _stepperMotorEnaPin;
    int _slowSpeed;
    int _maxSpeed;
    int _motorAcceleration;

public:
    LidStepperMotor(int stepperMotorDirPin, int stepperMotorPulPin, int stepperMotorEnaPin, int slowSpeed, int maxSpeed, int motorAcceleration);
    void Setup();
    void RaiseLid();
    void Stop();
    void Unspool();
    // void MotorStep(int speed);
    // void MoveStepper(bool motorDirection, int steps, int motorSpeed);
    // void MoveLid(bool motorDirection, int numSteps, bool normalOrSlowSpeed);
};

#endif // LidStepperMotor_h