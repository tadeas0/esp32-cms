#include "util.h"

void logRequest(AsyncWebServerRequest *request)
{
    Serial.println(request->methodToString() + String(" ") + request->url());
}