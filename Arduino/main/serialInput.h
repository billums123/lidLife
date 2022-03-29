/*Functions for user to input numbers or characters/strings into the serial monitor*/

#ifndef serialInput_h    
#define serialInput_h    

// Place your main header code here.

class serialInputClass
{
    public:
    serialInputClass();
    unsigned int RX_SetupData(String DataRequest);
    String RX_SetupData_String(String DataRequest);
};

extern serialInputClass serialInput;
#endif // serialCommunication_h    