#include "admin_handlers.h"

void postArticle(AsyncWebServerRequest *request)
{
    Serial.println("POST /article");
    ArticleManager articleManager(ARTICLE_DIR);
    if (!request->hasParam("text", true) || !request->hasParam("title", true))
    {
        request->send(400);
        return;
    }
    String text = request->getParam("text", true)->value();
    String title = request->getParam("title", true)->value();

    // TODO: Error handling
    articleManager.createArticle(text, title + ".md");
    String url = ADMIN_SCREEN_URI;
    url += "/index.html";
    request->redirect(url);
}

void deleteArticle(AsyncWebServerRequest *request)
{
    Serial.println("DELETE /article");
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    articleManager.deleteArticle(name + ".md");
    request->send(204);
}

void editArticle(AsyncWebServerRequest *request)
{
    Serial.println("GET /article/edit");
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    String page = articleManager.getEditableHTML(name);
    request->send(200, "text/html", page);
}

void connectToWifi(AsyncWebServerRequest *request)
{
    if (!request->hasParam("ssid", true) || !request->hasParam("password", true))
    {
        request->send(400);
        return;
    }

    request->send(200);

    String ssid = request->getParam("ssid")->value();
    String password = request->getParam("password")->value();

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