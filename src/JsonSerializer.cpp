//! \file

#include <json/json.h>

#include "Article.h"
#include "JsonSerializer.h"
#include "WalkerException.h"

namespace WikiWalker
{
  /*! Get article links in an array.
   * Basically undoing the Wikipedia to article conversion...
   * \param article pointer to article which links should be extracted
   * \return Json::Value array with titles as string
   * \todo should / could be a member function, but then I'd have to expose
   * Json::Value, which is ugly and clutters up other classes...
   */
  static Json::Value getArticleLinks(const Article* article)
  {
    Json::Value array(Json::ValueType::arrayValue);

    for(auto ali = article->linkBegin(); ali != article->linkEnd(); ali++) {
      auto a = ali->lock();
      if(a != nullptr) {
        std::string tit = a->title();
        array.append(Json::Value(tit));
      }
    }

    return array;
  }

  /*! Convert article to string representation of JSON representation.
   * \param a pointer to article to be converted
   * \return json as string
   * \internal uses the following format:
   * {"title":{"forward_links":[...]}, "title2":{"forward_links":[...]}, ...}
   */
  static std::string convertToJson(const ArticleCollection& ac)
  {
    Json::Value val(Json::ValueType::objectValue);

    for(auto ar : ac) {
      Json::Value linkObj(Json::ValueType::objectValue);

      if(ar.second->analyzed()) {
        linkObj["forward_links"] = getArticleLinks(ar.second.get());
      } else {
        linkObj["forward_links"] = Json::Value::nullSingleton();
      }

      val[ar.first] = linkObj;
    }

    Json::StreamWriterBuilder swb;
    swb["indentation"] = "";
    return Json::writeString(swb, val);
  }

  void JsonSerializer::serialize(const ArticleCollection& collection,
                                 std::ostream& outstream)
  {
    outstream << convertToJson(collection);
  }

  void JsonSerializer::deserialize(ArticleCollection& collection,
                                   std::istream& instream)
  {
    Json::Reader reader;
    Json::Value document;
    bool success = reader.parse(instream, document, false);

    if(!success) {
      throw WalkerException("Error parsing JSON");
    }

    // get all "main" articles first
    for(auto& titleElement : document.getMemberNames()) {
      std::string title = titleElement;

      //! \todo find a better solution than get-compare-add
      auto a = collection.get(title);

      if(a == nullptr) {
        a = std::make_shared<Article>(title);
        collection.add(a);
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
        std::shared_ptr<Article> la = collection.get(linkedTitle);

        if(la == nullptr) {
          la = std::make_shared<Article>(linkedTitle);
          collection.add(la);
        }

        a->addLink(la);
      }
    }
  }
}
