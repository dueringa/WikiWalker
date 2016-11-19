#include "JsonToArticleConverter.h"
#include "WalkerException.h"

#include <json/json.h>

//! \todo really ugly workaround, passing in the ArticleCollection instance... :/
Article* JsonToArticleConverter::convertToArticle(std::string json, ArticleCollection& articleCache)
{
    Json::Reader reader;
    Json::Value document;
    bool success = reader.parse(json, document, false);

    if(!success) {
        throw WalkerException("Error parsing JSON");
    }

    auto allPages = document.get("query", Json::Value::nullSingleton())
                            .get("pages", Json::Value::nullSingleton());

    // only get first page
    auto wantedPage = allPages.get(allPages.getMemberNames()[0],
                                   Json::Value::nullSingleton());

    //! \todo get normalized title, if present
    std::string wantedArticleTitle = wantedPage.get("title", Json::Value::nullSingleton()).asString();
    Article* wantedArticle = articleCache.get(wantedArticleTitle);

    if(wantedArticle == nullptr) {
        wantedArticle = new Article(wantedArticleTitle);
    }

    articleCache.add(wantedArticle);

    // add links
    for(const auto &linked : wantedPage.get("links", Json::Value::nullSingleton())) {
        auto linkedPageTitle = linked.get("title", Json::Value::nullSingleton()).asString();
        auto par = articleCache.get(linkedPageTitle);

        if(par == nullptr) {
            par = new Article(linkedPageTitle);
            articleCache.add(par);
        }

        wantedArticle->addLink(par);
    }

    wantedArticle->setAnalyzed(true);

    if(!document.isMember("batchcomplete")) {
        moreData = true;
        continueString =
          document.get("continue", Json::Value::nullSingleton())
                  .get("plcontinue", Json::Value::nullSingleton())
                    .asString();
    } else {
        moreData = false;
    }

    return wantedArticle;
}
