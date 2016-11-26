#include "CacheJsonToArticleConverter.h"

#include <json/json.h>

#include "WalkerException.h"
#include "Article.h"

ArticleCollection& CacheJsonToArticleConverter::convertToArticle(std::string json, ArticleCollection& articleCache)
{
    Json::Reader reader;
    Json::Value document;
    bool success = reader.parse(json, document, false);

    if(!success) {
        throw WalkerException("Error parsing JSON");
    }

    // get all "main" articles first
    for(auto &titleElement : document.getMemberNames()) {
        std::string title = titleElement;

        Article *a = articleCache.get(title);

        if(a == nullptr) {
            a = new Article(title);
            articleCache.add(a);
        }

        for(auto linkedArticle :
                document.get(title, Json::Value::nullSingleton())
                        .get("forward_links", Json::Value::nullSingleton())) {
            std::string linkedTitle = linkedArticle.asString();
            Article *la = articleCache.get(linkedTitle);

            if(la == nullptr) {
                la = new Article(linkedTitle);
                articleCache.add(la);
            }

            a->addLink(la);
        }
    }

    return articleCache;
    /*
        a->setAnalyzed(true); ?
    */
}
