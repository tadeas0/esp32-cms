#ifndef ADMIN_HANDLERS_H
#define ADMIN_HANDLERS_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "ArticleManager.h"
#include "constants.h"
#include "util.h"

void postArticle(AsyncWebServerRequest *request);
void deleteArticle(AsyncWebServerRequest *request);
void editArticle(AsyncWebServerRequest *request);
void connectToWifi(AsyncWebServerRequest *request);

#endif