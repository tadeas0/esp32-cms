#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "ArticleManager.h"

#define MAIN_SCREEN_URI "/screens"
#define ADMIN_SCREEN_URI "/admin-screens"
#define ARTICLE_URI "/article"
#define JSLIB_URI "/jslib"
#define MAIN_SCREEN_DIR "/screens"
#define ADMIN_SCREEN_DIR "/admin_screens"
#define ARTICLE_DIR "/articles"
#define JSLIB_DIR "/jslib"

ArticleManager articleManager("/articles");

void handleRoot(ESP8266WebServer &server)
{
    String url = MAIN_SCREEN_URI;
    url += "/index.html";
    server.sendHeader("Location", url, true);
    server.send(302);
}

void handleAdminRoot(ESP8266WebServer &server)
{
    String url = ADMIN_SCREEN_URI;
    url += "/index.html";
    server.sendHeader("Location", url, true);
    server.send(302);
}

void getArticles(ESP8266WebServer &server)
{
    String res = articleManager.getAllArticleMetadata(ARTICLE_URI);
    server.send(200, "text/html", res);
}

void getArticle(ESP8266WebServer &server)
{
    String name = server.pathArg(0);
    String content = server.arg("plain");
    Serial.println("GET /article/" + name);
    String page = articleManager.getArticleHTML(name);
    server.send(200, "text/html", page);
}

void postArticle(ESP8266WebServer &server)
{
    String name = server.pathArg(0);
    String content = server.arg("plain");
    String url = ADMIN_SCREEN_URI;
    url += "/index.html";
    articleManager.createArticle(content, name + ".md");
    server.sendHeader("Location", url, true);
    server.send(302);
}

void deleteArticle(ESP8266WebServer &server)
{
    String name = server.pathArg(0);
    Serial.println("DELETE /article/" + name);
    articleManager.deleteArticle(name + ".md");
    server.send(204);
}

void editArticle(ESP8266WebServer &server)
{
    String name = server.pathArg(0);
    Serial.println("GET /article/edit/" + name);
    String page = articleManager.getEditableHTML(name);
    server.send(200, "text/html", page);
}

void handleUnauthorized(ESP8266WebServer &server)
{
    server.send(403);
}

void connectToWifi(ESP8266WebServer &server)
{
    String content = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, content);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        server.send(400);
        return;
    }

    server.send(200);

    String ssid = doc["ssid"];
    String password = doc["password"];

    if (WiFi.isConnected())
        WiFi.disconnect();

    Serial.println("");
    WiFi.begin(ssid, password);
    while (true)
    {
        wl_status_t stat = WiFi.status();
        switch (stat)
        {
        case WL_CONNECTED:
            Serial.println("");
            Serial.print("Connected to: ");
            Serial.println(ssid);
            return;
            break;
        case WL_CONNECT_FAILED:
            Serial.println("");
            Serial.println("Connection failed");
            return;
            break;
        case WL_CONNECTION_LOST:
            Serial.println("");
            Serial.println("Connection lost");
            return;
            break;

        default:
            Serial.print(".");
            break;
        }
        delay(500);
    }
}