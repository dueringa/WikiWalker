//! \file CacheJsonToArticleConverter.cpp

#include "CacheJsonToArticleConverter.h"

#include <json/json.h>

#include "WalkerException.h"
#include "Article.h"

ArticleCollection& CacheJsonToArticleConverter::convertToArticle(
    const std::string& json,
    ArticleCollection& articleCache)
{
  Json::Reader reader;
  Json::Value document;
  bool success = reader.parse(json, document, false);

  if(!success) {
    throw WalkerException("Error parsing JSON");
  }

  // get all "main" articles first
  for(auto& titleElement : document.getMemberNames()) {
    std::string title = titleElement;

    //! \todo find a better solution than get-compare-add
    auto a = articleCache.get(title);

    if(a == nullptr) {
      a = std::make_shared<Article>(title);
      articleCache.add(a);
    }

    auto links = document.get(title, Json::Value::nullSingleton())
                     .get("forward_links", Json::Value::nullSingleton());

    if(links.isNull()) {
      /* don't need to set article analyzed to false,
       * since that's the default */
      continue;
    }

    a->setAnalyzed(true);

    for(const auto& linkedArticle : links) {
      std::string linkedTitle     = linkedArticle.asString();
      std::shared_ptr<Article> la = articleCache.get(linkedTitle);

      if(la == nullptr) {
        la = std::make_shared<Article>(linkedTitle);
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
