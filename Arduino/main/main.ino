
//---------------Includes---------------//
#include "HX711.h"
#include "serialInput.h"
#include "detentLinearActuator.h"
#include "lidLifterStrainGauge.h"

// HX711 circuit wiring
const int lidLiftStrainGauge_DOUT_pin = 2;
const int lidLiftStrainGauge_SCK_pin = 3;
const int tare_LED = 4;
//Detent Linear Actuator Variables
const int linActDir1 = 5;
const int linActDir2 = 6;
const int linActPinPWM = 10;
int linActExtendTime = 1500;
const int onBoardLED = 13;

// Detents Variables
bool detentsInUse;
HX711 scale;

void setup()
{
  Serial.begin(38400);
  pinMode(tare_LED, OUTPUT);
  detentLinearActuator.setup(linActDir1,linActDir2,linActPinPWM);
  lidLifterStrainGauge.setup(lidLiftStrainGauge_DOUT_pin,lidLiftStrainGauge_SCK_pin);

  pinMode(onBoardLED, OUTPUT);


  //  Serial.println("HX711 Demo");
  //
  //  Serial.println("Initializing the scale");

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  
  int startingCycleCount = serialInput.RX_SetupData_Int("Enter beginning cycle count:"); // Request cycle count input from user
  Serial.print("Starting count has been set to: ");
  Serial.println(startingCycleCount);
  delay(500);
  // while ((detentsInUse != true) || (detentsInUse != false))
  // {
    String detentsUsed = serialInput.RX_SetupData_String("Are detents being used in this test? (y/n)");
    bool detentsInUse = detentLinearActuator.checkIfDetentsAreUsed(detentsUsed);
    
  // }
  // Serial.print("Detents: ");
  // // Serial.println(detentsInUse);
  // scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // 401735
  // 271743.0
  // 1.1875

  //  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details


  // digitalWrite(tare_LED, HIGH);
  // scale.set_scale(-228836.2105);
  // // scale.set_scale();
  // scale.tare(); // reset the scale to 0
  // delay(1000);
  // digitalWrite(tare_LED, LOW);


  //  Serial.println("Place object of known weight on scale");
  //  delay(10000);
  //  Serial.println("Measuring weight...");
  //  delay(1000);
}

void loop()
{
  // if(det)
  detentLinearActuator.extend(linActDir1,linActDir2,linActPinPWM,linActExtendTime);
  // detentLinearActuator.retract(linActDir1,linActDir2,linActPinPWM,linActExtendTime);

  //  Serial.print("one reading:\t");
  // Serial.println(scale.get_units(), 2);
  // //  Serial.print("\t| average:\t");
  // //  Serial.println(scale.get_units(3), 2);

  // //  scale.power_down();             // put the ADC in sleep mode
  // //  delay(5000);
  // //  scale.power_up();
  // digitalWrite(onBoardLED, HIGH);
  // delay(100);
  // digitalWrite(onBoardLED, LOW);
  // delay(100);
  // digitalWrite(onBoardLED, HIGH);
  // delay(1000);
  // digitalWrite(onBoardLED, LOW);
  // delay(1000);

  
  // digitalWrite(motor1PWM, 255);
  // digitalWrite(motorControlPin1, HIGH);
  // digitalWrite(motorControlPin2, LOW);
  // delay(1500);
  // digitalWrite(motor1PWM, 0);
  // delay(1500);
  // digitalWrite(motor1PWM, 255);
  // digitalWrite(motorControlPin1, LOW);
  // digitalWrite(motorControlPin2, HIGH);
  // delay(1500);
  // digitalWrite(motor1PWM, 0);
  // delay(1500);
}