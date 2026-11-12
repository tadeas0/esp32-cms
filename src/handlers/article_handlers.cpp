#include "article_handlers.h"

void getArticles(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String res = articleManager.getAllArticleMetadata("/article");
    request->send(200, "text/html", res);
}

void getArticleHTML(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    String page = articleManager.getArticleHTML(name);
    request->send(200, "text/html", page);
}

void postArticle(AsyncWebServerRequest *request)
{
    logRequest(request);
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
    request->redirect("/");
}

void deleteArticle(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    articleManager.deleteArticle(name + ".md");
    request->send(204);
}

void editArticle(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    String page = articleManager.getEditableHTML(name);
    request->send(200, "text/html", page);
}

void getArticleMD(AsyncWebServerRequest *request)
{
    logRequest(request);
    ArticleManager articleManager(ARTICLE_DIR);
    String name = request->pathArg(0);
    String md = articleManager.getArticleMD(name);
    request->send(200, "text/plain", md);
}