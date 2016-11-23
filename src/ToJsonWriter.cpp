#include "ToJsonWriter.h"

#include <json/json.h>

Json::Value getArticleLinks(const Article* a)
{
    Json::Value array(Json::ValueType::arrayValue);

    for(auto ali = a->linkBegin(); ali != a->linkEnd(); ali++) {
        std::string tit = (*ali)->getTitle();
        array.append(Json::Value(tit));
    }

    return array;
}

std::string ToJsonWriter::convertToJson(const Article* a)
{
    Json::Value val;

    val[a->getTitle()] = getArticleLinks(a);

    Json::FastWriter jsw;
    jsw.omitEndingLineFeed();

    return jsw.write(val);
}

std::string ToJsonWriter::convertToJson(const ArticleCollection& ac)
{
    Json::Value val(Json::ValueType::objectValue);

    for(auto ar : ac) {
        val[ar.first] = getArticleLinks(ar.second);
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
