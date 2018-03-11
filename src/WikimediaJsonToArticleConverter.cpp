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

    for(auto& onePage : allPages) {
      //! get normalized title not necessary, "title" is already
      std::string oneTitle =
          onePage.get("title", Json::Value::nullSingleton()).asString();

      //! \todo find a better solution than get-compare-add
      auto wantedArticle = articleCache.get(oneTitle);

      if(wantedArticle == nullptr) {
        wantedArticle = std::make_shared<Article>(oneTitle);
        articleCache.add(wantedArticle);
      }

      if(onePage.isMember("missing") || onePage.isMember("invalid")) {
        wantedArticle->setMarked(true);
        wantedArticle->setAnalyzed(true);
        continue;
      }

      // add links
      std::shared_ptr<Article> par;
      for(const auto& linked :
          onePage.get("links", Json::Value::nullSingleton())) {
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
    }

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
}  // namespace WikiWalker