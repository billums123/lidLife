#include "StrainGauge.h"
#include "serialInput.h"

// #define DEBUG

double scaleReadout;
double scaleCalibrationValue;
float knownObjectWeight = 0;
float measuredForce;
char shouldStrainGaugeBeCalibrated;
// HX711 strainGaugeObject;

StrainGauge::StrainGauge(int strainGaugeDout, int strainGaugeClk, float strainGaugeCalibF, String strainGaugeName) : _strainGauge()
{
    _clk_pin = strainGaugeClk;
    _dout_pin = strainGaugeDout;
    _calibration_factor = strainGaugeCalibF;
    _strainGaugeName = strainGaugeName;
}

bool StrainGauge::Setup()
{
    pinMode(_dout_pin, OUTPUT);
    pinMode(_clk_pin, OUTPUT);
    _strainGauge.begin(_dout_pin, _clk_pin);
    _strainGauge.set_scale(_calibration_factor);
    Serial.print("Does the ");
    Serial.print(_strainGaugeName);
    Serial.print(" ");
    Serial.println("need to be calibrated? (y/n)");

    while (Serial.available() == 0)
    {
        // Wait for User to Input Data
    }

    while (true)
    {
        shouldStrainGaugeBeCalibrated = Serial.read();
        if ((shouldStrainGaugeBeCalibrated == 'y') || (shouldStrainGaugeBeCalibrated == 'Y') || (shouldStrainGaugeBeCalibrated == 'n') || (shouldStrainGaugeBeCalibrated == 'N'))
        {
            break;
        }
#ifdef DEBUG
        Serial.print("read: ");
        Serial.println(shouldStrainGaugeBeCalibrated);
#endif
    }
#ifdef DEBUG
    Serial.print(strainGaugeName);
    Serial.print(" Calibration User Input: ");
    Serial.println(shouldStrainGaugeBeCalibrated);
#endif
    if ((shouldStrainGaugeBeCalibrated == 'y') || (shouldStrainGaugeBeCalibrated == 'Y'))
    {
        return (true);
    }
    else if ((shouldStrainGaugeBeCalibrated == 'n') || (shouldStrainGaugeBeCalibrated == 'N'))
    {
        _strainGauge.tare();
        return (false);
    }
    else
    {
        Serial.println("Invalid character input, please try again.");
        Serial.println(_strainGaugeName);
       
    }
}

float StrainGauge::MeasureForce()
{
    measuredForce = _strainGauge.get_units(1), 2;
    return(measuredForce);
}
double StrainGauge::CalbrateScale()
{
    Serial.println("Remove objects from strain gauge");
    delay(5000);
    _strainGauge.tare();
    Serial.println("Place object of known weight on strain gauge");
    delay(1000);
    Serial.println("What is the weight of the known object (in grams)?");
    int i = 0;
    while (i == 0)
    {
        knownObjectWeight = Serial.parseFloat();
        if (knownObjectWeight > 0)
        {
            i = 1;
        }
    }
#ifdef DEBUG
    Serial.print("User's known weight input: ");
    Serial.println(knownObjectWeight);
#endif
    Serial.println("Weighing object...");
    scaleReadout = _strainGauge.get_value(10);                  // Measure the average value of 5 readings from the strain gauge
    scaleCalibrationValue = scaleReadout / (knownObjectWeight); // convert grams to pounds
    Serial.print("Your new calibration value is: ");
    Serial.println(scaleCalibrationValue);
    if (_strainGaugeName == "Lid Lifter")
    {
        Serial.println();
        Serial.println("****IMPORTANT!****Change the variable value for lidLifterCalibF in main.ino with the new calibration value, save file, and restart test.");
    }
    else if (_strainGaugeName == "Lid Pusher")
    {
        Serial.println();
        Serial.println("****IMPORTANT!****Change the variable value for lidPusherCalibF in main.ino with the new calibration value, save file, and restart test.");
    }
    else
    {
        Serial.println();
        Serial.println("****IMPORTANT!****Change the variable value for the strain gauge Calibration Factor in main.ino with the new calibration value, save file, and restart test.");
    }
}
