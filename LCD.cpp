#include "LCD.h"

LCD::LCD(int maxCol = 16, int maxRow = 2)  {
    this->maxCol = maxCol;
    this->maxRow = maxRow;
    this->curLine = 0;
    this->lcd = new LiquidCrystal(19, 23, 18, 17, 16, 15);
}

void LCD::initialize()  {
    this->lcd->begin(maxCol, maxRow);
    this->lcd->setCursor(0, 0);
}

template <typename T>
void LCD::print(T msg)  {
    this->lcd->print(msg);
}

template <typename T>
void LCD::println(T msg)  {
    this->lcd->print(msg);
    this->curLine++;
    if(this->curLine >= maxRow)
      this->curLine = 0;
    this->setCursor(0, curLine);
}

void LCD::setCursor(int col, int row)   {
    this->lcd->setCursor(col, row);
}

void LCD::clear() {
  int pixels = 80;
  this->setCursor(0, 0);
    while(pixels--)
      this->lcd->print(" ");
  this->curLine = 0;
}

void LCD::displayPowerProfile(float voltage, float current, float power)  {
//  this->setCursor(0, 0);
  this->print("Voltage: ");
  this->print(voltage);
  this->println("V");
  this->print("Current: ");
  this->print(current);
  this->println("mA");
  this->print("Power: ");
  this->print(power);
  this->println("mW");
}
