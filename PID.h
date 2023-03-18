#ifndef PID_H
#define PID_H

class PID {
  
  private:
    float kp;               // Proportional Constant
    float ki;               // Integral Constant
    float kd;               // Derivative Constant
    
    float integral;         // Integral value
    float derivative;       // Derivative value
    float lastActual;       // Last actual value of the system

    float setPoint;         // Desired value of the system
    float output;           // Generated output from PID controller

    float maxOut;           // Maximum value the system can withstand
    float minOut;           // Minimum value the system requires

    void _init();            // Initialize all the variables
    void clamp();           // Clamp the system between extreme points of the system


  public:
    PID();    // Default constructor
    PID(float setPoint, float kp, float ki, float kd, float minOut, float maxOut);    // Constructor to initialize the PID constant
    
    void setSetPoint(float setPoint);   // Set SetPoint of the system
    void setP(float kp);    // Set Proportional Constant
    void setI(float ki);    // Set Integral Constant
    void setD(float kd);    // Set Derivative Constant
    float getP();           // Get Proportional Constant
    float getI();           // Get Integral Constant
    float getD();           // Get Derivative Constant
    
    void setMaxOut(float maxOut);   // Set maximum value the system can withstand
    void setMinOut(float minOut);   // Set minimum value the system requires

    float feedback(float actual);   // Calculate the error and pushes the system towards desired value

    void clear();

};

#endif PID_H