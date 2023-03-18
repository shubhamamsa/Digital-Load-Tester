#ifndef LCD_H
#define LCD_H

#include<LiquidCrystal.h>

class LCD   {

    private:
        LiquidCrystal *lcd;
        int maxRow;
        int maxCol;
        int curLine;
        
    public:
        LCD(int maxCol, int maxRow);
        void initialize();
        template <typename T>
        void print(T msg);
        template <typename T>
        void println(T msg);
        void setCursor(int col, int row);
        void clear();
        void displayPowerProfile(float voltage, float current, float power);

};

#endif LCD_H