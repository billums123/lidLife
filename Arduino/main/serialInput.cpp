
#include "Arduino.h"
#include "serialInput.h"
int i = 1;
int m = 1;
serialInputClass::serialInputClass()
{
}

unsigned int serialInputClass::RX_SetupData(String DataRequest, String outputType)
{
  String inString = "";
  unsigned int rxInt = 0;
  bool dataReceived = false;

#ifdef DEBUG
  Serial.println(rxInt);
#endif

  while (dataReceived == false)
  {
    delay(1000); // Wait a second between requests to not overwhelm the pi
    if (i == 1)
    {
      Serial.println(DataRequest); // Sends the data request string
      i = 2;
    }
    delayMicroseconds(100); // Waits .1 of a second (to give the pi some time to respond)
    while (Serial.available() == 0)
    {
    }
    if (Serial.available() > 0)
    {
      char inChar = Serial.read(); // Reads the serial port for input
#ifdef DEBUG
      Serial.print("read byte: ");
      Serial.println(inChar);
#endif
      if (isDigit(inChar)) // make sure received a digit and add to the string
      {
        inString += (char)inChar;
#ifdef DEBUG
        Serial.print("inString: ");
        Serial.println(inString);
#endif
      }
      else if (inChar == '\n' && inString[0] != 0) // once all the digits have been received
      {                                            // convert to an integer and exit
      if (outputType == "float"){
        rxInt = inString.toFloat();
      }
      else{
        rxInt = inString.toInt();
      }
        dataReceived = true;
#ifdef DEBUG
        Serial.print(DataRequest);
        Serial.print(" ");
        Serial.println(rxInt);
#endif
      }
    }
  }
  return (rxInt);
}

String serialInputClass::RX_SetupData_String(String DataRequest)
{
  String rxString = "";
  bool dataReceived = false;

#ifdef DEBUG
  Serial.println(rxString);
#endif

  while (dataReceived == false)
  {
    delay(1000); // Wait a second between requests to not overwhelm the pi
    if (m == 1)
    {
      Serial.println(DataRequest); // Sends the data request string
      m = 2;
    }
    delayMicroseconds(100); // Waits .1 of a second (to give the pi some time to respond)
    if (Serial.available() > 0)
    {
      char rxChar = Serial.read(); // Reads the serial port for input
#ifdef DEBUG
      Serial.print("read byte: ");
      Serial.println(rxChar);
#endif
      if (rxChar == '\n' && rxString[0] != 0) // a non-empty string has been received and exit
      {
        dataReceived = true;
        break;
#ifdef DEBUG
        Serial.print(DataRequest);
        Serial.print(" ");
        Serial.println(rxString);
#endif
      }
      rxString += (char)rxChar; // Append character to the string
#ifdef DEBUG
      Serial.print("rxString: ");
      Serial.println(rxString);
#endif
    }
  }
  return (rxString);
}

serialInputClass serialInput = serialInputClass();
