#include "PID.h"

PID::PID()  {
  _init();
}

PID::PID(float setPoint, float kp, float ki, float kd, float minOut, float maxOut)  {
  _init();
  this->setPoint = setPoint;
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  this->minOut = minOut;
  this->maxOut = maxOut;
}

void PID::_init()  {
  this->kp = 0.0;
  this->ki = 0.0;
  this->kd = 0.0;
  this->integral = 0.0;
  this->derivative = 0.0;
  this->lastActual = 0.0;
  this->setPoint = 0.0;
  this->output = 0.0;
  this->maxOut = 0.0;
  this->minOut = 0.0;
}

void PID::clamp() {
  if(this->output > this->maxOut)
    this->output = this->maxOut;
  else if(this->output < this->minOut)
    this->output = this->minOut;
}

void PID::setSetPoint(float setPoint) {
  this->setPoint = setPoint;
}

void PID::setP(float kp)  {
  this->kp = kp;
}

void PID::setI(float ki)  {
  this->ki = ki;
}

void PID::setD(float kd)  {
  this->kd = kd;
}

float PID::getP() {
  return this->kp;
}

float PID::getI() {
  return this->ki;
}

float PID::getD() {
  return this->kd;
}

void PID::setMaxOut(float maxOut)  {
  this->maxOut = maxOut;
}

void PID::setMinOut(float minOut)  {
  this->minOut = minOut;
}

float PID::feedback(float actual)  {
  float error = this->setPoint - actual;
  this->integral = this->integral + error;
  this->derivative = (actual - this->lastActual);
  this->output = this->kp*error + this->ki*this->integral + this->kd*this->derivative;
  this->lastActual = actual;
  clamp();
  return this->output;
}

void PID::clear() {
  this->setP(0);
  this->setI(0);
  this->setD(0);
  this->integral = 0;
  this->derivative = 0;
  this->lastActual = 0;
}