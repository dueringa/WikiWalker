//! \file WikimediaJsonToArticleConverter.cpp

#include "WikimediaJsonToArticleConverter.h"

#include <json/json.h>

#include "Article.h"
#include "WalkerException.h"

namespace WikiWalker
{
  //! \todo really ugly workaround, passing in the ArticleCollection instance...
  //! :/
  WikimediaJsonToArticleConverter::ContinuationStatus
  WikimediaJsonToArticleConverter::convertToArticle(
      const std::string& json,
      ArticleCollection& articleCache)
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
    auto wantedPage =
        allPages.get((Json::ArrayIndex)0, Json::Value::nullSingleton());

    if(wantedPage.isMember("missing")) {
      throw WalkerException("Article doesn't exist");
    } else if(wantedPage.isMember("invalid")) {
      throw WalkerException("Invalid article");
    }

    //! get normalized title not necessary, "title" is already
    std::string wantedArticleTitle =
        wantedPage.get("title", Json::Value::nullSingleton()).asString();

    //! \todo find a better solution than get-compare-add
    auto wantedArticle = articleCache.get(wantedArticleTitle);

    if(wantedArticle == nullptr) {
      wantedArticle = std::make_shared<Article>(wantedArticleTitle);
    }

    articleCache.add(wantedArticle);

    // add links
    std::shared_ptr<Article> par;
    for(const auto& linked :
        wantedPage.get("links", Json::Value::nullSingleton())) {
      auto linkedPageTitle =
          linked.get("title", Json::Value::nullSingleton()).asString();
      par = articleCache.get(linkedPageTitle);

      if(par == nullptr) {
        par = std::make_shared<Article>(linkedPageTitle);
        articleCache.add(par);
      }

      wantedArticle->addLink(par);
    }

    wantedArticle->setAnalyzed(true);

    bool moreData;

    if(!document.isMember("batchcomplete")) {
      moreData       = true;
      continueString = document.get("continue", Json::Value::nullSingleton())
                           .get("plcontinue", Json::Value::nullSingleton())
                           .asString();
    } else {
      moreData       = false;
      continueString = "";
    }

    return moreData ? ContinuationStatus::ConversionNeedsMoreData
                    : ContinuationStatus::ConversionCompleted;
  }
}