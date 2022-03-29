

//---------------Includes---------------//
#include "HX711.h"
#include "serialInput.h"
#include "DetentLinearActuator.h"
#include "StrainGauge.h"
#include "LidStepperMotor.h"
#include "IRSensors.h"
int z = 0;
// #define DEBUG
#define runSetupPrompts
// Count Variables
int startingCycleCount;
int cycleCount;
bool beginCycleTimer = true;
unsigned long raiseCycleTime;
unsigned long lowerCycleTime;
bool lidAtClosedPosition;
bool lidAtOpenPosition= false; //variable for keeping track of point in cycle
// int z = 0;
// int q = 0;

// Lid Lifter Motor Variables
const int lidStepperMotorDirPin = 2;        // LidStepperMotor Direction
const int lidStepperMotorPulPin = 3;        // LidStepperMotor Pulse Pin
const int lidStepperMotorEnaPin = 4;        // LidStepperMotor On/Off Toggle
const int lidStepperMotorStepsPerRev = 400; // LidStepperMotor Steps per revolution
const int lidMotorSlowSpeed = 1000;
const int lidMotorMaxSpeed = 10000;
const int lidMotorAcceleration = 15000;
int numStepsRaiseLid = 21000;
int numStepsLowerLid = 0;
float lidLiftForceArray[5];
int t = 0; // indexing variable for array

LidStepperMotor LidLifterStepperMotor(lidStepperMotorDirPin, lidStepperMotorPulPin, lidStepperMotorEnaPin, lidMotorSlowSpeed, lidMotorMaxSpeed, lidMotorAcceleration);

// Lid Lifter Strain Gauge Variablel
float lidLifterCalibF = -440.02;
bool lidLifterCalibrationNeeded;
const int lidLiftStrainGauge_DOUT_pin = 5;
const int lidLiftStrainGauge_SCK_pin = 6;
int numOfMeasurements = 5;
int measurementFrequency = numStepsRaiseLid / numOfMeasurements;
int delayBetweenMeasurements = 500;
String lidLifterStrainGaugeName = "Lid Lifter";
StrainGauge LidLifterStrainGauge(lidLiftStrainGauge_DOUT_pin, lidLiftStrainGauge_SCK_pin, lidLifterCalibF, lidLifterStrainGaugeName);

// Lid Pusher Strain Gauge Variables
double lidPusherCalibF = 466.21;
bool lidPusherCalibrationNeeded;
const int lidPushStrainGauge_DOUT_pin = 7;
const int lidPushStrainGauge_SCK_pin = 8;
String lidPusherStrainGaugeName = "Lid Pusher";
StrainGauge LidPusherStrainGauge(lidPushStrainGauge_DOUT_pin, lidPushStrainGauge_SCK_pin, lidPusherCalibF, lidPusherStrainGaugeName);
// const int onBoardLED = 13;

// Detent Linear Actuator Variables
bool detentsInUse;
const int linActDir1 = 9;
const int linActDir2 = 10;
const int linActPinPWM = 11;
int linActSetupExtendTime = 12000;
int linActSetupRetractTime = 2000;
int linActExtendTime = 2250;
int linActRetractTime = 8000;
DetentLinearActuator DetentPushLinearActuator(linActDir1, linActDir2, linActPinPWM);

// IRSensorVariables
const int IRSensorBottomPin = A0;
const int IRSensorTopPin = A1;


IRSensors CycleAndTimerCountIRSensors(IRSensorBottomPin, IRSensorTopPin);
void setup()
{
  Serial.begin(230400);
  // startClock = millis();
  // pinMode(tare_LED, OUTPUT);
  // Serial.println("Setting up");
  DetentPushLinearActuator.Setup();
  LidLifterStepperMotor.Setup();
  CycleAndTimerCountIRSensors.Setup();
#ifdef runSetupPrompts
  lidLifterCalibrationNeeded = LidLifterStrainGauge.Setup();

  // Check if calibration is needed for the lid lifter strain gauge
  if (lidLifterCalibrationNeeded == true)
  {
    LidLifterStrainGauge.CalbrateScale();
  }
#endif

  // Check if calibration is needed for the lid pusher strain gauge
#ifdef runSetupPrompts
  lidPusherCalibrationNeeded = LidPusherStrainGauge.Setup();
  if (lidPusherCalibrationNeeded == true)
  {
    LidPusherStrainGauge.CalbrateScale();
  }
#endif

  if (CycleAndTimerCountIRSensors.SenseOpen() == 0)
  { // If the lid is open on startup, push it over to the closed position
    DetentPushLinearActuator.Extend();
    delay(linActSetupExtendTime);
  }
  DetentPushLinearActuator.Retract();

  // Setup for lid lifter strain gauge
#ifdef DEBUG

  Serial.print("lifter: ");
  Serial.println(lidLifterCalibrationNeeded);
#endif

  // Setup for lid pusher strain gauge
#ifdef DEBUG
  Serial.print("pusher: ");
  Serial.println(lidPusherCalibrationNeeded);
#endif

  // #ifdef runSetupPrompts
  startingCycleCount = serialInput.RX_SetupData("Enter beginning cycle count:"); // Request cycle count input from user
  cycleCount = startingCycleCount; //assign the input cycle count from user

  // Put Linear Actuator in Starting Position
  DetentPushLinearActuator.Extend();
  delay(2250);
  DetentPushLinearActuator.Stop();
}

void loop()
{
  
//RUN THE CODE BELOW AS THE LID IS BEING RAISED
if (lidAtOpenPosition == false){
  if (CycleAndTimerCountIRSensors.SenseOpen() == 1)
  {

    if (beginCycleTimer == true)
    {
      CycleAndTimerCountIRSensors.StartTimer();
      Serial.println("TimerSTarted");
    }
    beginCycleTimer = false;
    LidLifterStepperMotor.moveTo(numStepsRaiseLid);
    LidLifterStepperMotor.RaiseLid();
    if (((z % measurementFrequency) == 0) && (z != 0))
    {
      lidLiftForceArray[t] = LidLifterStrainGauge.MeasureForce(); // safe lid lift forces into array
      t++;
    }
    z++;
  }
  if (CycleAndTimerCountIRSensors.SenseOpen() == 0) // lid in open position
  {
    if (beginCycleTimer == false)
    {
      raiseCycleTime = CycleAndTimerCountIRSensors.StopTimer();
      Serial.print("cycle time: ");
      Serial.println(raiseCycleTime);
      
      // Serial.println(lidLiftForceArray[0]);
      // Serial.println(lidLiftForceArray[1]);
      // Serial.println(lidLiftForceArray[2]);
      // Serial.println(lidLiftForceArray[3]);
      // Serial.println(lidLiftForceArray[4]);
    }
    
    beginCycleTimer = true;
    // DetentPushLinearActuator.Extend(); 
    // delay(2500);
    lidAtOpenPosition = true;
    z=0; //reset cycle count used for force measurements
  }
  }
  //END OF LID RAISING CODE

  //START OF LOWERING LID CODE
  Serial.println(lidAtOpenPosition);
  if (lidAtOpenPosition == true){
    LidLifterStepperMotor.moveTo(numStepsRaiseLid);
    LidLifterStepperMotor.Unspool();
    // DetentPushLinearActuator.Extend(); 
    if (LidPusherStrainGauge.MeasureForce()>1){//Wait until force is being measured to store data
      // Serial.print("Push FOrce: ");Serial.println(LidPusherStrainGauge.MeasureForce());
    }
  }

  //   LidLifterStepperMotor.Unspool(numStepsLowerLid);
  //   // Serial.println("TestLower");
  //   // Serial.print("cycle time: "); Serial.println(raiseCycleTime);


}