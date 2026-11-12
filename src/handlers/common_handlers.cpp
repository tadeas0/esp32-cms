#include "common_handlers.h"

void getArticles(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String res = articleManager.getAllArticleMetadata("/article");
    request->send(200, "text/html", res);
}

void getArticle(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    String page = articleManager.getArticleHTML(name);
    request->send(200, "text/html", page);
}

void sendBadRequest(AsyncWebServerRequest *request)
{
    logRequest(request);
    request->send(400);
}