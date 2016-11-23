#include "ToJsonWriter.h"

#include <json/json.h>

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
        std::string tit = (*ali)->getTitle();
        array.append(Json::Value(tit));
    }

    return array;
}

std::string ToJsonWriter::convertToJson(const Article* a)
{
    Json::Value val(Json::ValueType::objectValue);

    Json::Value linkObj(Json::ValueType::objectValue);
    linkObj["forward_links"] = getArticleLinks(a);
    val[a->getTitle()] = linkObj;

    Json::FastWriter jsw;
    jsw.omitEndingLineFeed();

    return jsw.write(val);
}

std::string ToJsonWriter::convertToJson(const ArticleCollection& ac)
{
    Json::Value val(Json::ValueType::objectValue);

    for(auto ar : ac) {
        Json::Value linkObj(Json::ValueType::objectValue);
        linkObj["forward_links"] = getArticleLinks(ar.second);
        val[ar.first] = linkObj;
    }

    Json::FastWriter jsw;
    jsw.omitEndingLineFeed();

    return jsw.write(val);
}

void ToJsonWriter::output(const Article* article, std::ostream& outstream)
{
    outstream << convertToJson(article);
}

void ToJsonWriter::output(const ArticleCollection& collection, std::ostream& outstream)
{
    outstream << convertToJson(collection);
}
