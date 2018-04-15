//! \file

#include <json/json.h>

#include "Article.h"
#include "JsonSerializer.h"
#include "WalkerException.h"

namespace WikiWalker
{
  namespace JsonSerializerInformation
  {
    constexpr const char* SchemeVersionName = "scheme-version";
    static int SchemeVersion                = 2;

    constexpr const char* ProgramKeyName = "program";
    constexpr const char* ProgramValue   = "wikiwalker";

    constexpr const char* CollectionKey = "ArticleCollection";
  }  // namespace JsonSerializerInformation

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
      // nullptrs in articles will simply be *omitted*
      // else {
      //   // do nothing!
      // }
    }

    return array;
  }

  /*! Convert article to string representation of JSON representation.
   * \param ac reference to articlecollection to be converted
   * \return json as string
   * \internal uses the following format:
   * {"title":{"forward_links":[...]}, "title2":{"forward_links":[...]}, ...}
   */
  static std::string convertToJson(const CollectionUtils::ArticleCollection& ac)
  {
    Json::Value header(Json::ValueType::objectValue);
    header[JsonSerializerInformation::ProgramKeyName] =
        JsonSerializerInformation::ProgramValue;
    header[JsonSerializerInformation::SchemeVersionName] =
        JsonSerializerInformation::SchemeVersion;

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
    header[JsonSerializerInformation::CollectionKey] = val;

    Json::StreamWriterBuilder swb;
    swb["indentation"] = "";
    return Json::writeString(swb, header);
  }

  void JsonSerializer::serialize(
      const CollectionUtils::ArticleCollection& collection,
      std::ostream& outstream)
  {
    outstream << convertToJson(collection);
  }

  CollectionUtils::ArticleCollection JsonSerializer::deserialize(
      std::istream& instream)
  {
    CollectionUtils::ArticleCollection ac;
    deserialize(ac, instream);
    return ac;
  }

  void JsonSerializer::deserialize(
      CollectionUtils::ArticleCollection& collection,
      std::istream& instream)
  {
    Json::Value document;
    Json::CharReaderBuilder crb;
    Json::CharReaderBuilder::strictMode(&crb.settings_);
    bool success = Json::parseFromStream(crb, instream, &document, nullptr);

    if(!success) {
      throw WalkerException("Error parsing JSON");
    }

    if(!document.isObject()) {
      throw WalkerException("Error: Json root is not an object");
    }

    {
      auto programName = document.get(JsonSerializerInformation::ProgramKeyName,
                                      Json::Value::nullSingleton());
      if(programName.isNull() || !programName.isString() ||
         programName.asString() != JsonSerializerInformation::ProgramValue) {
        throw WalkerException("Error: Wrong program name");
      }
    }
    {
      auto schemeVersion =
          document.get(JsonSerializerInformation::SchemeVersionName,
                       Json::Value::nullSingleton());
      if(schemeVersion.isNull() || !schemeVersion.isNumeric() ||
         schemeVersion.asLargestUInt() !=
             JsonSerializerInformation::SchemeVersion) {
        throw WalkerException("Error: Wrong scheme version");
      }
    }

    auto coll = document.get(JsonSerializerInformation::CollectionKey,
                             Json::Value::nullSingleton());
    if(coll.isNull() || !coll.isObject()) {
      throw WalkerException("Error: collection is not serialized correctly");
    }

    // get all "main" articles first
    for(auto& titleElement : coll.getMemberNames()) {
      std::string title = titleElement;

      //! \todo find a better solution than get-compare-add
      auto a = CollectionUtils::get(collection, title);

      if(a == nullptr) {
        a = std::make_shared<Article>(title);
        CollectionUtils::add(collection, a);
      }

      auto links = coll.get(title, Json::Value::nullSingleton())
                       .get("forward_links", Json::Value::nullSingleton());

      if(links.isNull()) {
        /* don't need to set article analyzed to false,
         * since that's the default */
        continue;
      }

      a->analyzed(true);

      for(const auto& linkedArticle : links) {
        std::string linkedTitle = linkedArticle.asString();
        std::shared_ptr<Article> la =
            CollectionUtils::get(collection, linkedTitle);

        if(la == nullptr) {
          la = std::make_shared<Article>(linkedTitle);
          CollectionUtils::add(collection, la);
        }

        a->addLink(la);
      }
    }
  }
}  // namespace WikiWalker
