#ifndef COMMON_HANDLERS_H
#define COMMON_HANDLERS_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "ArticleManager.h"
#include "constants.h"
#include "util.h"

void getArticles(AsyncWebServerRequest *request);
void getArticleHTML(AsyncWebServerRequest *request);
void postArticle(AsyncWebServerRequest *request);
void deleteArticle(AsyncWebServerRequest *request);
void editArticle(AsyncWebServerRequest *request);
void getArticleMD(AsyncWebServerRequest *request);

#endif