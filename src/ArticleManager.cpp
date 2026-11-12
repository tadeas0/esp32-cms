#include "ArticleManager.h"

ArticleManager::ArticleManager(String basePath) : basePath(basePath) {}

String ArticleManager::getAllArticleMetadata(String baseLink)
{
    Serial.println("Getting article metadata");
    Dir dir = LittleFS.openDir(basePath);
    String res = "[";
    while (dir.next())
    {
        String filename = dir.fileName();
        String name = filename;
        name.replace(".md", "");
        String path = this->basePath + dir.fileName();
        if (res.length() > 1)
            res += ",";
        res += "{";
        res += "\"name\":\"" + name + "\",";
        res += "\"link\":\"" + baseLink + "/" + name + ".html\",";
        res += "\"editLink\":\"" + baseLink + "/edit/" + name + "\"";
        res += "}";
    }
    res += "]";
    return res;
}

String ArticleManager::getArticleHTML(String name)
{
    Serial.println("Getting article " + name);
    String temp_path = ARTICLE_TEMPLATE_PATH;

    // TODO: error handling
    File templ = LittleFS.open(temp_path, "r");
    String templStr = templ.readString();
    templStr.replace(TITLE_VAR, name);
    templ.close();
    return templStr;
}

String ArticleManager::getArticleMD(String name)
{
    Serial.println("Getting article MD " + name);
    // TODO: error handling
    File mdFile = LittleFS.open(basePath + "/" + name + ".md", "r");
    String md = mdFile.readString();
    mdFile.close();
    return md;
}

String ArticleManager::getEditableHTML(String articleName)
{
    Serial.println("Getting editable article " + articleName);
    File editTempl = LittleFS.open(EDIT_TEMPLATE_PATH, "r");
    String editStr = editTempl.readString();
    editStr.replace(TITLE_VAR, articleName);
    editTempl.close();
    return editStr;
}

void ArticleManager::createArticle(String content, String filename)
{
    Serial.println("Creating article " + filename);
    // TODO: error handling
    File f = LittleFS.open(basePath + "/" + filename, "w");
    f.write(content.c_str());
    f.close();
    Serial.println("Created article " + filename);
}

void ArticleManager::deleteArticle(String filename)
{
    Serial.println("Deleting article " + filename);
    // TODO: error handling
    LittleFS.remove(basePath + "/" + filename);
    Serial.println("Deleted article " + filename);
}
