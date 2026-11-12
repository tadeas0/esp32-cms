#include "wifi_handlers.h"

void connectToWifi(AsyncWebServerRequest *request)
{
    logRequest(request);
    if (!request->hasParam("ssid", true) || !request->hasParam("password", true))
    {
        request->send(400);
        return;
    }

    request->redirect("/admin-screens/wifi.html");

    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();
    theWifiManager().connect(ssid, password);
}

void deleteConnection(AsyncWebServerRequest *request)
{
    logRequest(request);
    if (!request->hasParam("ssid", true))
    {
        request->send(400);
        return;
    }
    String ssid = request->getParam("ssid", true)->value();
    if (theWifiManager().deleteConnection(ssid))
    {
        request->send(204);
    }
    else
    {
        request->send(400);
    }
}

void getWifi(AsyncWebServerRequest *request)
{
    logRequest(request);
    String resJson = theWifiManager().getNetworksJson();
    request->send(200, "application/json", resJson);
}

void getCurrentConn(AsyncWebServerRequest *request)
{
    logRequest(request);
    String resJson = theWifiManager().getCurrentConnJson();
    request->send(200, "application/json", resJson);
}