
//---------------Includes---------------//
#include "HX711.h"
#include "serialInput.h"
#include "detentLinearActuator.h"
#include "StrainGauge.h"

// #define DEBUG

// Count Variables
int startingCycleCount = 0;

const int tare_LED = 4;

// Lid Lifter Strain Gauge Variables
float lidLifterCalibF = -440.02;
bool lidLifterCalibrationNeeded;
const int lidLiftStrainGauge_DOUT_pin = 2;
const int lidLiftStrainGauge_SCK_pin = 3;
String lidLifterStrainGaugeName = "Lid Lifter";
StrainGauge lidLifterStrainGauge(lidLiftStrainGauge_DOUT_pin,lidLiftStrainGauge_SCK_pin,lidLifterCalibF,lidLifterStrainGaugeName) ;

// Lid Pusher Strain Gauge Variables
double lidPusherCalibF = 1000;
bool lidPusherCalibrationNeeded;
const int lidPushStrainGauge_DOUT_pin = 11;
const int lidPushStrainGauge_SCK_pin = 1;
String lidPusherStrainGaugeName = "Lid Pusher";
StrainGauge lidPusherStrainGauge(lidPushStrainGauge_DOUT_pin,lidPushStrainGauge_SCK_pin,lidPusherCalibF,lidPusherStrainGaugeName) ;
const int onBoardLED = 13;

// Detents Variables
bool detentsInUse;

// Detent Linear Actuator Variables
const int linActDir1 = 5;
const int linActDir2 = 6;
const int linActPinPWM = 10;
int linActExtendTime = 1500;
int linActRetractTime = 1500;

void setup()
{
  Serial.begin(38400);
  pinMode(tare_LED, OUTPUT);
  detentLinearActuator.setup(linActDir1, linActDir2, linActPinPWM);

  // Setup for lid lifter strain gauge
  lidLifterCalibrationNeeded = lidLifterStrainGauge.Setup();
  Serial.print("lifter: ");
  Serial.println(lidLifterCalibrationNeeded);
  if (lidLifterCalibrationNeeded == true)
  {
    lidLifterStrainGauge.CalbrateScale();
  }
 
  // Setup for lid pusher strain gauge
  lidPusherCalibrationNeeded = lidPusherStrainGauge.Setup();
  Serial.print("pusher: ");
  Serial.println(lidPusherCalibrationNeeded);
  if (lidPusherCalibrationNeeded == true)
  {
    lidPusherStrainGauge.CalbrateScale();
  }

  startingCycleCount = serialInput.RX_SetupData("Enter beginning cycle count:", "integer"); // Request cycle count input from user
  Serial.print("Starting count has been set to: ");
  Serial.println(startingCycleCount);
delay(500);
  String detentsUsed = serialInput.RX_SetupData_String("Are detents being used in this test? (y/n)");
#ifdef DEBUG
  Serial.print("User detent input: ");
  Serial.println(detentsUsed);
#endif
  bool detentsInUse = detentLinearActuator.checkIfDetentsAreUsed(detentsUsed);
#ifdef DEBUG
  Serial.print("Detents Used? ");
  Serial.println(detentsInUse);
#endif
}

void loop()
{
  // if(det)
  detentLinearActuator.extend(linActDir1, linActDir2, linActPinPWM, linActExtendTime);
  lidLifterStrainGauge.MeasureForce();
  lidPusherStrainGauge.MeasureForce();
  delay(500);
  // detentLinearActuator.retract(linActDir1,linActDir2,linActPinPWM,linActRetractTime);

  // //  scale.power_down();             // put the ADC in sleep mode
  // //  delay(5000);
  // //  scale.power_up();
}