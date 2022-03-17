#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int tare_LED = 4;

const int motorControlPin1 = 5;
const int motorControlPin2 = 6;
const int motor1PWM = 10;

HX711 scale;

void setup() {
  Serial.begin(38400);
  pinMode(tare_LED,OUTPUT);
  pinMode(motorControlPin1,OUTPUT);
  pinMode(motorControlPin2,OUTPUT);
  pinMode(motor1PWM,OUTPUT);
  
//  Serial.println("HX711 Demo");
//
//  Serial.println("Initializing the scale");

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

//401735
//271743.0
//1.1875

  //  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  digitalWrite(tare_LED,HIGH);
  scale.set_scale(-228836.2105);
//scale.set_scale();
  scale.tare();               // reset the scale to 0
  delay(1000);
  digitalWrite(tare_LED,LOW);
//  Serial.println("Place object of known weight on scale");
//  delay(10000);
//  Serial.println("Measuring weight...");
//  delay(1000); 

}

void loop() {
//  Serial.print("one reading:\t");
  Serial.println(scale.get_units(), 2);
//  Serial.print("\t| average:\t");
//  Serial.println(scale.get_units(3), 2);

//  scale.power_down();             // put the ADC in sleep mode
//  delay(5000);
//  scale.power_up();
digitalWrite(motor1PWM, 255);
digitalWrite(motorControlPin1, HIGH);
digitalWrite(motorControlPin2, LOW);
delay(1500);
digitalWrite(motor1PWM, 0);
delay(1500);
digitalWrite(motor1PWM, 255);
digitalWrite(motorControlPin1, LOW);
digitalWrite(motorControlPin2, HIGH);
delay(1500);
digitalWrite(motor1PWM, 0);
delay(1500);
}
