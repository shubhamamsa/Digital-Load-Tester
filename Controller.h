#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP4725.h> 
#include "PID.h"

class Controller  {

  private:
    // ADC
    Adafruit_ADS1115 ads1115;
    float voltageMultiplier;
    float currentMultiplier;
    float shuntResistance;

    // DAC
    Adafruit_MCP4725 MCP4725;
    float voltageLevel;

    // Power Profile
    float voltage, current, power, thresholdVoltage;
    int currentMode;

    // PID Controller
    PID *pid;
    float dCurrent;
    float dPower;
    float dResistance;

  public:
    Controller();
    void initialize();
    void execute();
//    void constantPowerMode();
    void readVoltage();
    void readCurrent();
    void calculatePower();
    void setDesiredCurrent(float);
    void setDesiredPower(float);
    void setDesiredResistance(float);
    float getCurrent();
    float getVoltage(); 
    float getPower();
    void setVoltageLevel(int voltageLevel);
    void printPowerProfile();
    bool setMode(int);

    // Temporary functions will be deprecated in future
    void setP(float kp);
    void setI(float ki);
    void setD(float kd);
    void clear();
};

#endif CONTROLLER_H
