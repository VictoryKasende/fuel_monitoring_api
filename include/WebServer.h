#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "TankManager.h"

class FuelMonitoringServer {
private:
    AsyncWebServer server;
    TankManager* tankManager;
    
    void handleRoot(AsyncWebServerRequest *request);
    void handleAPIStatus(AsyncWebServerRequest *request);
    void handleAPIAllTanks(AsyncWebServerRequest *request);
    void handleAPITank(AsyncWebServerRequest *request);
    void handleNotFound(AsyncWebServerRequest *request);
    void setCORSHeaders(AsyncWebServerResponse *response);
    
public:
    FuelMonitoringServer(int port, TankManager* tm);
    void init();
    bool isConnected();
    String getIPAddress();
};

#endif // WEB_SERVER_H
