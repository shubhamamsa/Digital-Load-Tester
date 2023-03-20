#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <WiFi.h>
#include<time.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include<HTTPClient.h>
#include <uri/UriBraces.h>
#include "Controller.h"

class WiFiService   {

    private:
        const char *ssid;
        const char *password;
        
        const char* elasticServer;
        
        const char* ntpServer;
        unsigned long long epochTime;
        unsigned long initialMillis;
        
        static WebServer *server;
        static Controller *control;
        
        static void pidHandler();
        static void currentHandler();
        static void powerHandler();
        static void resistanceHandler();
        static void resetHandler();
        static void handleNotFound();
        static float sToF(String num);
        unsigned long getInitialEpochTime();
    
    public:
        WiFiService(const char *ssid, const char *password, Controller *control);
        void initialize();
        void connect();
        static void listen();
        unsigned long long getEpochTime();
        void publishData();
};

#endif WIFI_SERVICE_H
