#include "Controller.h" 
#include "LCD.h"
#include "WiFiService.h"

// Attributes

// WiFi Credentials
char *ssid = "shubham";
char *password = "12345671";

// Output Time interval
long long stime = millis();

// Main controller
Controller control;

// LCD
LCD lcd(20, 4);

// Webserver & Client
WiFiService service(ssid, password, &control);

void setup()  {
  Serial.begin(115200);
  control.initialize();
  lcd.initialize();
  service.initialize();
}

void loop() {
  service.listen();
  delay(2);
  control.execute();
  if(millis() - stime > 1000) {
    lcd.clear();
    control.printPowerProfile();
    lcd.displayPowerProfile(control.getVoltage(), control.getCurrent(), control.getPower());
    service.publishData();
    stime = millis();
  }
}
