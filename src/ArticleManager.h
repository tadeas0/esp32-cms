#ifndef ARTICLE_MANAGER_H
#define ARTICLE_MANAGER_H

#include <LittleFS.h>

#define ARTICLE_TEMPLATE_PATH "/screens/article.html"
#define EDIT_TEMPLATE_PATH "/admin_screens/edit_article.html"
#define CONTENT_VAR "%content%"
#define TITLE_VAR "%title%"

class ArticleManager
{
public:
    ArticleManager(String basePath);
    String getAllArticleMetadata(String baseLink);
    String getArticleHTML(String name);
    String getArticleMD(String name);
    String getEditableHTML(String articleName);
    void createArticle(String content, String filename);
    void deleteArticle(String filename);

private:
    String basePath;
};

#endif