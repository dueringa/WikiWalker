//! \file WikimediaJsonToArticleConverter.cpp

#include "WikimediaJsonToArticleConverter.h"

#include <cassert>
#include <sstream>

#include <json/json.h>

#include "Article.h"
#include "WalkerException.h"

namespace WikiWalker
{
  //! \todo really ugly workaround, passing in the
  //! CollectionUtils::ArticleCollection instance...
  //! :/
  WikimediaJsonToArticleConverter::ContinuationStatus
  WikimediaJsonToArticleConverter::convert(
      const std::string& json,
      CollectionUtils::ArticleCollection& articleCache)
  {
    Json::Value document;
    Json::CharReaderBuilder crb;
    Json::CharReaderBuilder::strictMode(&crb.settings_);
    std::istringstream jsonStream(json);
    bool success = Json::parseFromStream(crb, jsonStream, &document, nullptr);

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
      auto wantedArticle = CollectionUtils::get(articleCache, oneTitle);

      if(wantedArticle == nullptr) {
        wantedArticle = std::make_shared<Article>(oneTitle);
        CollectionUtils::add(articleCache, wantedArticle);
      }

      if(onePage.isMember("missing") || onePage.isMember("invalid")) {
        wantedArticle->marked(true);
        wantedArticle->analyzed(true);
        continue;
      }

      // add links
      //! \todo support linkshere
      std::shared_ptr<Article> par;
      if(onePage.isMember("links")) {
        for(const auto& linked :
            onePage.get("links", Json::Value::nullSingleton())) {
          auto linkedPageTitle =
              linked.get("title", Json::Value::nullSingleton()).asString();
          par = CollectionUtils::get(articleCache, linkedPageTitle);

          if(par == nullptr) {
            par = std::make_shared<Article>(linkedPageTitle);
            CollectionUtils::add(articleCache, par);
          }

          wantedArticle->addLink(par);
        }

        wantedArticle->analyzed(true);
      }
    }

    bool moreData;

    // always clear, otherwise insert won't happen
    continuationData_.clear();

    if(document.isMember("continue")) {
      moreData = true;
      const auto contData =
          document.get("continue", Json::Value::nullSingleton());
      assert(contData.isObject());

      for(auto it = contData.begin(); it != contData.end(); it++) {
        continuationData_.emplace(it.name(), it->asString());
      }
    } else {
      moreData = false;
    }

    return moreData ? ContinuationStatus::ConversionNeedsMoreData
                    : ContinuationStatus::ConversionCompleted;
  }
}  // namespace WikiWalker
