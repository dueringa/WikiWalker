//! \file CacheJsonToArticleConverter.cpp

#include "CacheJsonToArticleConverter.h"

#include <memory>

#include <json/json.h>

#include "Article.h"
#include "WalkerException.h"

namespace WikiWalker
{
  ArticleCollection& CacheJsonToArticleConverter::convert(
      std::istream& json,
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

      a->analyzed(true);

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
     * a->analyzed(true); ?
     */
  }

  ArticleCollection& CacheJsonToArticleConverter::convert(
      const std::string& json,
      ArticleCollection& articleCache)
  {
    std::istringstream jsonStream(json);
    return convert(jsonStream, articleCache);
  }
}  // namespace WikiWalker