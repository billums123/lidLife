

// LidStepperMotor::LidStepperMotor(int stepperMotorDirPin, int stepperMotorPulPin, int stepperMotorEnaPin, int stepperMotorstepsPerRev, int normalMotorSpeed, int slowMotorSpeed)
// {
//     _stepperMotorDirPin = stepperMotorDirPin;
//     _stepperMotorPulPin = stepperMotorPulPin;
//     _stepperMotorEnaPin = stepperMotorEnaPin;
//     _stepperMotorStepsPerRev = stepperMotorstepsPerRev;
//     _normalMotorSpeed = normalMotorSpeed;
//     _slowMotorSpeed = slowMotorSpeed;
// }

// void LidStepperMotor::Setup(){
//     pinMode(_stepperMotorDirPin, OUTPUT);
//     pinMode(_stepperMotorPulPin, OUTPUT);
//     pinMode(_stepperMotorEnaPin, OUTPUT);
// }

// /**********Motor Command**********/
// void LidStepperMotor::MotorStep(int speed){
//   digitalWrite(_stepperMotorPulPin, HIGH);
//   delayMicroseconds(speed);
//   digitalWrite(_stepperMotorPulPin, LOW);
//   delayMicroseconds(speed);
// }

// void LidStepperMotor::MoveStepper(bool motorDirection, int numSteps, int motorSpeed) // Motor Pull pin location, direction pin location,
// {
//     /*
//       inputs:
//               dir: Bool [0 or 1] for clockwise or counter clockwise, might have to play around to know your input
//               steps: int, how many steps to you want to go.
//               Speed: int, [600-3000] adjusts motor speed
//       Output:
//               None

//     */
//     int CurStep = 0; // set an initialization of steps
//     if (motorDirection == 0)
//     {
//         digitalWrite(_stepperMotorDirPin, LOW);
//     }
//     else
//     {
//         digitalWrite(_stepperMotorDirPin, HIGH);
//     }
//     digitalWrite(_stepperMotorEnaPin, LOW);
//     while (CurStep < numSteps)
//     {
//         MotorStep(motorSpeed);
//         CurStep++;
//     }
//     digitalWrite(_stepperMotorEnaPin, HIGH);
// }

// void LidStepperMotor::MoveLid(bool motorDirection,int numSteps, bool normalOrSlowSpeed)
// {
// /*
//   Inputs:
//           Direction: Bool [0 or 1] for clockwise or counter clockwise, might have to play around to know your input
//           NumStep: int, how many steps to you want to go.

//   */
// if (normalOrSlowSpeed == 1){
//   MoveStepper(motorDirection,numSteps, _normalMotorSpeed);
// }
//   else {
//     MoveStepper(motorDirection,numSteps, _slowMotorSpeed);
//   }

// }

#include "LidStepperMotor.h"

const int dirPin = 2;
const int stepPin = 3;
const int ENA = 4;
int Limit_1 = A1;

LidStepperMotor::LidStepperMotor(int stepperMotorDirPin, int stepperMotorPulPin, int stepperMotorEnaPin, int slowSpeed, int maxSpeed, int motorAcceleration) : AccelStepper(DRIVER, stepperMotorPulPin, stepperMotorDirPin)
{
  _stepperMotorDirPin = stepperMotorDirPin;
  _stepperMotorPulPin = stepperMotorPulPin;
  _stepperMotorEnaPin = stepperMotorEnaPin;
  
  _slowSpeed = slowSpeed;
  _maxSpeed = maxSpeed;
  _motorAcceleration = motorAcceleration;
}

void LidStepperMotor::Setup()
{
  pinMode(_stepperMotorEnaPin, OUTPUT); // ENA, enable pin is set to OUTPUT
  setMaxSpeed(_maxSpeed); // this limits the value of setSpeed(). Raise it if you like. 15000 does not work
  setSpeed(4000);
  setAcceleration(_motorAcceleration);
  digitalWrite(_stepperMotorEnaPin, HIGH); // ENA pin is switched LOW, in code to initially stop the _stepperMotor from moving.
  // runSpeed(); // This will run the motor dependant on which 'if' statement is active.
}

void LidStepperMotor::RaiseLid()
{
  digitalWrite(_stepperMotorEnaPin, LOW); // Switch ENA Pin High, Stepper Runs
  setSpeed(_maxSpeed);
  runSpeedToPosition();
}

void LidStepperMotor::Stop() //Stop stepper motor
{
  digitalWrite(_stepperMotorEnaPin, HIGH);
}

void LidStepperMotor::Unspool()
{
  digitalWrite(_stepperMotorEnaPin, LOW); // Switch ENA Pin High, Stepper Runs
  setSpeed(-_maxSpeed);      // reverse direction
  runSpeedToPosition();
}

