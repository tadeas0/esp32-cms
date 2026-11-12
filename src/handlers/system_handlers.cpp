#include "system_handlers.h"

void getSystemInfo(AsyncWebServerRequest *request)
{
    logRequest(request);
    FSInfo info;
    LittleFS.info(info);
    String res;
    res += "{";
    res += "\"used_bytes\":" + String(info.usedBytes) + ",";
    res += "\"total_bytes\":" + String(info.totalBytes) + ",";
    res += "\"ssid\":\"" + WiFi.softAPSSID() + "\",";
    res += "\"password\":\"" + WiFi.softAPPSK() + "\",";
    res += "\"dns_name\":\"" + theDNSManager().getName() + "\"";
    res += "}";
    request->send(200, "application/json", res);
}

void changeSystemSettings(AsyncWebServerRequest *request)
{
    logRequest(request);
    if (request->hasParam("ssid", true))
    {
        bool res = false;
        String ssid = request->getParam("ssid", true)->value();
        Serial.println(ssid);
        if (ssid.length() > 0)
        {
            if (request->hasParam("password"), true)
            {
                String pass = request->getParam("password", true)->value();
                if (ssid != WiFi.softAPSSID() || pass != WiFi.softAPPSK())
                {
                    res = WiFi.softAP(ssid, pass);
                }
            }
            else if (ssid != WiFi.softAPSSID())
            {
                res = WiFi.softAP(ssid);
            }
            if (res)
            {
                Serial.println("Changed AP credentials. New SSID: " + ssid);
            }
            else
            {
                Serial.println("Failed to change AP credentials.");
            }
        }
    }
    if (request->hasParam("dns_name", true))
    {
        String dnsName = request->getParam("dns_name", true)->value();
        if (dnsName.length() > 0)
        {
            if (dnsName != theDNSManager().getName())
            {
                theDNSManager().start(dnsName);
            }
        }
    }
    request->redirect("/");
}