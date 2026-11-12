#ifndef COMMON_HANDLERS_H
#define COMMON_HANDLERS_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "ArticleManager.h"
#include "constants.h"
#include "util.h"

void getArticles(AsyncWebServerRequest *request);
void getArticle(AsyncWebServerRequest *request);

#endif