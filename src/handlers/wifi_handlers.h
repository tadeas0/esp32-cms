#include "ESPAsyncWebServer.h"
#include "util.h"
#include "WifiManager.h"

void connectToWifi(AsyncWebServerRequest *request);
void deleteConnection(AsyncWebServerRequest *request);
void getWifi(AsyncWebServerRequest *request);
void getCurrentConn(AsyncWebServerRequest *request);