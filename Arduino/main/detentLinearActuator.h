/*Functions for user to control the linear actuator used to measure closing force of lid*/

#ifndef detentLinearActuator_h    
#define detentLinearActuator_h    

// Place your main header code here.

class detentLinearActuatorClass
{
    public:
    detentLinearActuatorClass();
    bool checkIfDetentsAreUsed(String detentUseStatus);
    void setup(int linActDir1, int linActDir2, int linActPinPWM);
    void extend(int linActDir1, int linActDir2, int linActPinPWM, int extensionTime);
    void retract(int linActDir1, int linActDir2, int linActPinPWM, int rectractionTime);
};

extern detentLinearActuatorClass detentLinearActuator;
#endif // detentLinearAct_h    