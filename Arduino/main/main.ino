//---------------Includes---------------//
#include "HX711.h"
#include "serialInput.h"
#include "DetentLinearActuator.h"
#include "StrainGauge.h"
#include "LidStepperMotor.h"
#include "IRSensors.h"

// #define DEBUG
#define runSetupPrompts
// Count Variables
int startingCycleCount;
int cycleCount;
bool beginCycleTimer = true;
unsigned long raiseCycleTime;
unsigned long lowerCycleTime;
bool lidAtClosedPosition;
bool lidAtOpenPosition = false; // variable for keeping track of point in cycle
bool cycleComplete = false;     // used to signal complete cycle
String cycleData;               // output for cycle data
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
int numStepsRaiseLid = 24000;
int numStepsLowerLid = 0;

LidStepperMotor LidLifterStepperMotor(lidStepperMotorDirPin, lidStepperMotorPulPin, lidStepperMotorEnaPin, lidMotorSlowSpeed, lidMotorMaxSpeed, lidMotorAcceleration);

// Lid Lifter Strain Gauge Variable
int z = 0; // variable for keeping track of cycle count, used for lifter and pusher strain gauges
float lidLifterCalibF = -440.02;
bool lidLifterCalibrationNeeded;
const int lidLiftStrainGauge_DOUT_pin = 5;
const int lidLiftStrainGauge_SCK_pin = 6;
int numOfLifterStrainGaugeMeasurements = 5;
int lidLifterMeasurementFrequency = numStepsRaiseLid / numOfLifterStrainGaugeMeasurements;
float lidLiftForceArray[5];
int t = 0; // indexing variable for both Lifter and pusher force arrays
String lidLifterStrainGaugeName = "Lid Lifter";
StrainGauge LidLifterStrainGauge(lidLiftStrainGauge_DOUT_pin, lidLiftStrainGauge_SCK_pin, lidLifterCalibF, lidLifterStrainGaugeName);

// Lid Pusher Strain Gauge Variables
double lidPusherCalibF = 466.21;
bool lidPusherCalibrationNeeded;
const int lidPushStrainGauge_DOUT_pin = 7;
const int lidPushStrainGauge_SCK_pin = 8;
int numOfIterationsDuringMeasurement = 120;
int numOfPusherStrainGaugeMeasurements = 10;
int lidPusherMeasurementFrequency = numOfIterationsDuringMeasurement / numOfPusherStrainGaugeMeasurements;
float lidPushForceArray[10];
String lidPusherStrainGaugeName = "Lid Pusher";
StrainGauge LidPusherStrainGauge(lidPushStrainGauge_DOUT_pin, lidPushStrainGauge_SCK_pin, lidPusherCalibF, lidPusherStrainGaugeName);
// const int onBoardLED = 13;

// Detent Linear Actuator Variables
bool detentsInUse;
const int linActDir1 = 9;
const int linActDir2 = 10;
const int linActPinPWM = 11;
int linActSetupExtendTime = 12000;
int linActSetupRetractTime = 12000;
int linActExtendTime = 2250;
int linActRetractTime = 10000;
DetentLinearActuator DetentPushLinearActuator(linActDir1, linActDir2, linActPinPWM);

// IRSensorVariables
const int IRSensorBottomPin = A0;
const int IRSensorTopPin = A1;

IRSensors CycleAndTimerCountIRSensors(IRSensorBottomPin, IRSensorTopPin);
void setup()
{
  Serial.begin(230400);
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
  cycleCount = startingCycleCount;                                               // assign the input cycle count from user

  // Put Linear Actuator in Starting Position
  DetentPushLinearActuator.Extend();
  delay(1000);
  DetentPushLinearActuator.Stop();
  // Format Serial Output
  Serial.println();
  Serial.print("Cycle Number\t");
  Serial.print("Lid Lowering Time\t");
  Serial.print("LiftF1\tLiftF2\tLiftF3\tLiftF4\tLiftF5\t");
  Serial.println("PushF1\tPushF2\tPushF3\tPushF4\tPushF5\tPushF6\tPushF7\tPushF8\tPushF9\tPushF10\t");
}

void loop()
{

  //////////////// RUN THE CODE BELOW AS THE LID IS BEING RAISED //////////////
  if (lidAtOpenPosition == false)
  {
    if (CycleAndTimerCountIRSensors.SenseOpen() == 1)
    {
      LidLifterStepperMotor.moveTo(numStepsRaiseLid);
      LidLifterStepperMotor.RaiseLid();
      if (((z % lidLifterMeasurementFrequency) == 0) && (z != 0))
      {
        lidLiftForceArray[t] = LidLifterStrainGauge.MeasureForce(); // safe lid lift forces into array
        t++;
      }
      z++;
    }
    if (CycleAndTimerCountIRSensors.SenseOpen() == 0) // lid in open position
    {
      lidAtOpenPosition = true;
      z = 0; // reset cycle count used for force measurements
      t = 0; // reset cycle count used for force array allocation
    }
  }
  //////////////// END OF LID RAISING CODE //////////////

  //////////////// START OF LOWERING LID CODE  //////////////
  if (lidAtOpenPosition == true)
  {
    LidLifterStepperMotor.moveTo(numStepsLowerLid);
    LidLifterStepperMotor.Unspool();
    DetentPushLinearActuator.Extend();
    if (LidLifterStepperMotor.currentPosition() == 0)
    {
      for (t; t < numOfPusherStrainGaugeMeasurements; t++)
      {
        lidPushForceArray[t] = LidPusherStrainGauge.MeasureForce(); // safe lid lift forces into array
        if (lidPushForceArray[t] > 5000)
        { // If there's an unspooling issue, retract linear actuator and stop test
          DetentPushLinearActuator.Retract();
          while (true)
          {
            Serial.println("Error unspooling");
            delay(1000);
          }
        }
        delay(500);
      }

      if ((CycleAndTimerCountIRSensors.SenseOpen() == 1) &&(beginCycleTimer == true))
      {                                           // Start time once lid begins to close
        CycleAndTimerCountIRSensors.StartTimer(); // start timer
        beginCycleTimer = false; //prevent timer from restarting
      }
      if (CycleAndTimerCountIRSensors.SenseClosed() == 0)
      {
        lowerCycleTime = CycleAndTimerCountIRSensors.StopTimer(); // stop the timer
        DetentPushLinearActuator.Retract();                       // begin to retract linear actuator
        lidAtOpenPosition = false;                                // change to false so cycle will start again
        delay(linActRetractTime);                                 // Wait for linear actuator to retract
        cycleComplete = true;                                     // end of one cycle
      }
    }
  }

  //////////////// END OF LOWERING LID CODE  //////////////

  ////////////Start of cycle data transmission ////////////////////
  if (cycleComplete == true)
  {
    t = 0;        // reassign cycling variables
    z = 0;        // reassign cycling variables
    beginCycleTimer = true; //reset cycle timer flag
    cycleCount++; // increase cycle count
    Serial.print(cycleCount);
    Serial.print("\t");
    Serial.print(lowerCycleTime);
    Serial.print("\t");
    for (z = 0; z < numOfLifterStrainGaugeMeasurements; z++)
    {
      Serial.print(lidLiftForceArray[z]);
      Serial.print("\t");
    }
    for (z = 0; z < numOfPusherStrainGaugeMeasurements; z++)
    {
      Serial.print(lidPushForceArray[z]);
      Serial.print("\t");
    }
    Serial.println();
    cycleComplete = false;
    /////////// end of cyle data transmission, start new cycle/////////////
  }

}