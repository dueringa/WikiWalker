//! \file

#include <json/json.h>

#include "Article.h"
#include "JsonSerializer.h"

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
}
