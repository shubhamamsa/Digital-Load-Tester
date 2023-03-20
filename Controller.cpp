#include "Controller.h"

Controller::Controller()
  : voltageMultiplier(0.9275),
    currentMultiplier(0.1875),
    shuntResistance(1.1),
    dCurrent(0.0),
    dPower(0.0),
    dResistance(0.0),
    thresholdVoltage(3.0),
    currentMode(0)
{
  pid = new PID(0.0, 0.4, 1.0, 0.1, 0.0, 4095.0);
}

void Controller::initialize() {
  while(!ads1115.begin())  {
    Serial.println("ADC fuk gya");
  }
  while(!MCP4725.begin(0x60))  {
    Serial.println("DAC fuk gya");
  }
  MCP4725.setVoltage(0, false);
}

void Controller::execute() {
  readVoltage();
  readCurrent();
  calculatePower();
  float curVal;
  switch(this->currentMode) {
    case 0:
      curVal = this->current;
      this->pid->setSetPoint(dCurrent);
      break;
    case 1:
      curVal = this->power / this->voltage;
      this->pid->setSetPoint(this->dPower / this->voltage);
      break;
    case 2:
      curVal = this->voltage / this->current;
      this->pid->setSetPoint(this->dResistance);
  }
  voltageLevel = pid->feedback(curVal);
  if(thresholdVoltage > voltage)
    MCP4725.setVoltage(0, false);
  else  
    MCP4725.setVoltage((int)voltageLevel, false);
}

void Controller::readVoltage() {
  int16_t adc0 = ads1115.readADC_SingleEnded(0);
  voltage = ((float)adc0 * voltageMultiplier) / 1000.0;
}

void Controller::readCurrent() {
  int16_t adc1 = ads1115.readADC_SingleEnded(1);
  current = (((float)adc1 * currentMultiplier) / shuntResistance);
}

void Controller::setDesiredCurrent(float dCurrent) {
  this->dCurrent = dCurrent;
}

void Controller::setDesiredPower(float dPower) {
  this->dPower = dPower;
}

void Controller::setDesiredResistance(float dResistance) {
  this->dResistance = dResistance;
}

void Controller::calculatePower() {
  this->power = this->voltage * this->current;
}

float Controller::getCurrent()  {
  return this->current;  
}

float Controller::getVoltage()  {
  return this->voltage;
}

float Controller::getPower()  {
  return this->power;
}

void Controller::setVoltageLevel(int voltageLevel)  {
  MCP4725.setVoltage(voltageLevel, false);
}

bool Controller::setMode(int num) {
  if(num < 0 || num > 1)
    return false;
  this->currentMode = num;
  return true;
}

void Controller::setP(float kp)  {
  return this->pid->setP(kp);
}

void Controller::setI(float ki)  {
  return this->pid->setI(ki);
}

void Controller::setD(float kd)  {
  return this->pid->setD(kd);
}

void Controller::clear()  {
  this->pid->clear();
}

void Controller::printPowerProfile()  {
  Serial.print("Voltage: ");
  Serial.println(this->voltage);
  Serial.print("Current: ");
  Serial.println(this->current);
  Serial.print("Power: ");
  Serial.println(this->power);
  Serial.print("Error: ");
  Serial.println(this->dCurrent - this->current);
  Serial.print("DAC: ");
  Serial.println(this->voltageLevel);
  Serial.print("dCurrent: ");
  Serial.println(this->dCurrent);
  Serial.print("dPower: ");
  Serial.println(this->dPower);
  Serial.print("Mode: ");
  Serial.println(this->currentMode);
  Serial.println();
  Serial.println();
  
}
