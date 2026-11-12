#ifndef ADMIN_HANDLERS_H
#define ADMIN_HANDLERS_H

// #include <ESP8266WebServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "ArticleManager.h"
#include "constants.h"

// void handleAdminRoot(ESP8266WebServer &server);
void postArticle(AsyncWebServerRequest *request);
void deleteArticle(AsyncWebServerRequest *request);
void editArticle(AsyncWebServerRequest *request);
void connectToWifi(AsyncWebServerRequest *request);

#endif