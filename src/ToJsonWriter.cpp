#include "ToJsonWriter.h"

#include <json/json.h>

std::string ToJsonWriter::convertToJson(const Article* a)
{
    Json::Value val;

    auto array = val[a->getTitle()];

    for(auto ali = a->linkBegin(); ali != a->linkEnd(); ali++) {
        std::string tit = (*ali)->getTitle();
        array.append(Json::Value(tit));
    }

    val[a->getTitle()] = array;

    Json::FastWriter jsw;

    return jsw.write(val);
}

std::string ToJsonWriter::convertToJson(const ArticleCollection&)
{
    return "";
}

void ToJsonWriter::output(const Article* article, std::ostream& outstream)
{
    outstream << convertToJson(article);
}

void ToJsonWriter::output(const ArticleCollection& collection, std::ostream& outstream)
{
    outstream << convertToJson(collection);
}
