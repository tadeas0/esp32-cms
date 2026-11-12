#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <uri/UriBraces.h>
#include "handlers/wifi_handlers.h"
#include "handlers/article_handlers.h"
#include "handlers/system_handlers.h"
#include "WifiManager.h"
#include "DNSManager.h"
#include "constants.h"

AsyncWebServer server(80);

void setup(void)
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(INNERIP, IPAddress(0, 0, 0, 0), IPAddress(255, 255, 255, 0));
    WiFi.softAP(APSSID, APPSK);

    Serial.print("Created AP: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    Serial.println("Mounting filesystem");
    if (!LittleFS.begin())
    {
        Serial.println("Could not mount the filesystem...");
        delay(2000);
        ESP.restart();
    }
    Serial.println("Mounted filesystem");

    Serial.println("Setting up DNS server");
    theDNSManager().start(DOMAIN_NAME);
    Serial.println("DNS server started");

    theWifiManager().loadCredsFromFile(WIFI_CREDS);
    theWifiManager().autoConnect();

    // Static
    server.serveStatic("/jslib", LittleFS, JSLIB_DIR);
    server.serveStatic("/js", LittleFS, JS_DIR);
    server.serveStatic("/css", LittleFS, CSS_DIR);
    server.serveStatic("/icons", LittleFS, ICONS_DIR);
    server.serveStatic("/admin-screens", LittleFS, ADMIN_SCREEN_DIR).setFilter(ON_AP_FILTER);
    server.serveStatic("/screens", LittleFS, MAIN_SCREEN_DIR).setFilter(ON_STA_FILTER);
    server.rewrite("/", "/screens/index.html").setFilter(ON_STA_FILTER);
    server.rewrite("/", "/admin-screens/index.html").setFilter(ON_AP_FILTER);

    // AP Paths
    server.on("^/article\\/edit\\/(.+)$", HTTP_GET, editArticle).setFilter(ON_AP_FILTER);
    server.on("^/article\\/(.+)$", HTTP_DELETE, deleteArticle).setFilter(ON_AP_FILTER);
    server.on("/article", HTTP_POST, postArticle).setFilter(ON_AP_FILTER);
    server.on("/wifi/current", HTTP_GET, getCurrentConn).setFilter(ON_AP_FILTER);
    server.on("/wifi", HTTP_DELETE, deleteConnection).setFilter(ON_AP_FILTER);
    server.on("/wifi", HTTP_POST, connectToWifi).setFilter(ON_AP_FILTER);
    server.on("/wifi", HTTP_GET, getWifi).setFilter(ON_AP_FILTER);
    server.on("/system", HTTP_GET, getSystemInfo).setFilter(ON_AP_FILTER);
    server.on("/system", HTTP_POST, changeSystemSettings).setFilter(ON_AP_FILTER);

    // STA Paths
    server.on("^\\/article\\/(.+).html$", HTTP_GET, getArticleHTML);
    server.on("^\\/article\\/(.+).md$", HTTP_GET, getArticleMD);
    server.on("/article", HTTP_GET, getArticles);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    theDNSManager().processNextRequest();
}
