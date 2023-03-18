#include "WiFiService.h"

WebServer* WiFiService::server = new WebServer(80);
Controller* WiFiService::control;

WiFiService::WiFiService(const char *ssid, const char *password, Controller *control)    {
    this->ssid = ssid;
    this->password = password;
    this->control = control;
    this->elasticServer = "http://192.168.1.10/power_profile";
    this->ntpServer = "pool.ntp.org";
    this->epochTime = 0;
}

void WiFiService::initialize()  {
    this->connect();
    if (MDNS.begin("esp32")) {
        Serial.println("MDNS responder started");
    }
    server->on(UriBraces("/set/kp/{}/ki/{}/kd/{}/current/{}"), pidHandler);
    server->on("/reset", resetHandler);
    server->on(UriBraces("/setCurrent/{}"), currentHandler);
    server->on(UriBraces("/setPower/{}"), powerHandler);
    server->onNotFound(handleNotFound);
    server->begin();
    Serial.println("HTTP server started");
    configTime(0, 0, ntpServer);
    while(epochTime < 100000)
      epochTime = getInitialEpochTime();
    epochTime *= 1000;
    initialMillis = millis();
}

void WiFiService::connect() {
    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(this->ssid, this->password);
    Serial.println("\nConnecting");
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void WiFiService::pidHandler() {
    String arg0 = server->pathArg(0);
    String arg1 = server->pathArg(1);
    String arg2 = server->pathArg(2);
    String arg3 = server->pathArg(3);
    float kp = sToF(arg0);
    float ki = sToF(arg1);
    float kd = sToF(arg2);
    float current = sToF(arg3);
    control->setDesiredCurrent(current);
    control->setP(kp);
    control->setI(ki);
    control->setD(kd);
    Serial.println("Kp: " + arg0 + "\nKi: " + arg1 + "\nKd: " + arg2 + "\nCurrent: " + arg3);
    server->send(200, "text/plain", "Kp: " + arg0 + "\nKi: " + arg1 + "\nKd: " + arg2 + "\nCurrent: " + arg3);  
}

void WiFiService::resetHandler()  {
  control->setVoltageLevel(0);
  control->setDesiredCurrent(0.0);
  control->setDesiredPower(0.0);
  server->send(200, "text/plain", "Reset done");
}

void WiFiService::currentHandler()  {
  String arg0 = server->pathArg(0);
  float current = sToF(arg0);
  control->setMode(0);
  control->setDesiredCurrent(current);
  server->send(200, "text/plain", "Current mode on\nCurrent set: "+arg0);
}

void WiFiService::powerHandler()  {
  String arg0 = server->pathArg(0);
  float power = sToF(arg0);
  control->setMode(1);
  control->setDesiredPower(power);
  server->send(200, "text/plain", "Power mode on\nPower set: "+arg0);
}

void WiFiService::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void WiFiService::listen()  {
  server->handleClient();
}

unsigned long WiFiService::getInitialEpochTime() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL)!= 0)
        return 0;
    return tv.tv_sec;
}

unsigned long long WiFiService::getEpochTime() {
    return (epochTime + (millis() - initialMillis));
}

void WiFiService::publishData() {
    WiFiClient client;
    HTTPClient http;
    DynamicJsonDocument data(1024);
    String body;
    data["timestamp"] = this->getEpochTime();
    data["voltage"] = control->getVoltage();
    data["current"] = control->getCurrent() / 1000.0;
    data["power"] = control->getPower() / 1000.0;
    serializeJson(data, body);
    Serial.println(body);
    if(WiFi.status()== WL_CONNECTED){
        http.begin(client, elasticServer);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(body); 
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
          
        // Free resources
        http.end();
    }
    else
      Serial.println("WiFi Disconnected");
}

float WiFiService::sToF(String num) {
    return num.toFloat();
}
