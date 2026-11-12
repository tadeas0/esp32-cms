#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFiClient.h>
#include "util.h"
#include "DNSManager.h"

void getSystemInfo(AsyncWebServerRequest *request);
void changeSystemSettings(AsyncWebServerRequest *request);