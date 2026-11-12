#include "common_handlers.h"

void getArticles(AsyncWebServerRequest *request)
{
    Serial.println("GET " + request->url());
    ArticleManager articleManager(ARTICLE_DIR);
    String res = articleManager.getAllArticleMetadata(ARTICLE_URI);
    request->send(200, "text/html", res);
}

void getArticle(AsyncWebServerRequest *request)
{
    Serial.println("GET " + request->url());
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    String page = articleManager.getArticleHTML(name);
    request->send(200, "text/html", page);
}

void sendBadRequest(AsyncWebServerRequest *request)
{
    request->send(400);
}