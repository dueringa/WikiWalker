//! \file WikimediaJsonToArticleConverter.cpp

#include "WikimediaJsonToArticleConverter.h"

#include <sstream>

#include <json/json.h>

#include "Article.h"
#include "WalkerException.h"

namespace WikiWalker
{
  //! \todo really ugly workaround, passing in the ArticleCollection instance...
  //! :/
  WikimediaJsonToArticleConverter::ContinuationStatus
  WikimediaJsonToArticleConverter::convert(const std::string& json,
                                           ArticleCollection& articleCache)
  {
    Json::Value document;
    Json::CharReaderBuilder crb;
    crb.strictMode(&crb.settings_);
    bool success = Json::parseFromStream(
        crb, std::istringstream(json), &document, nullptr);

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
        wantedArticle->marked(true);
        wantedArticle->analyzed(true);
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

      wantedArticle->analyzed(true);
    }

    bool moreData;

    if(!document.isMember("batchcomplete")) {
      moreData        = true;
      continueString_ = document.get("continue", Json::Value::nullSingleton())
                            .get("plcontinue", Json::Value::nullSingleton())
                            .asString();
    } else {
      moreData        = false;
      continueString_ = "";
    }

    return moreData ? ContinuationStatus::ConversionNeedsMoreData
                    : ContinuationStatus::ConversionCompleted;
  }
}  // namespace WikiWalker