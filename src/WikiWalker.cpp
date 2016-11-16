#include "WikiWalker.h"

#include <iostream>
#include <json/json.h>

Article getArticle(std::string json)
{
    Json::Reader reader;
    Json::Value document;
    bool success = reader.parse(json, document, false);

    if(!success) {
        throw WalkerException("Error parsing JSON");
    }

    auto allPages = document.get("query", Json::Value::nullSingleton())
                            .get("pages", Json::Value::nullSingleton());

    // only get first page
    auto wantedPage = allPages.get(allPages.getMemberNames()[0],
                                   Json::Value::nullSingleton());

    Article wantedArticle(wantedPage.get("title", Json::Value::nullSingleton()).asString());

    // add links
    for(const auto &linked : wantedPage.get("links", Json::Value::nullSingleton()))
    {
        wantedArticle.addLink(
            new Article(linked.get("title", Json::Value::nullSingleton()).asString())
        );
    }

    return wantedArticle;
}

void WikiWalker::startWalking(std::string url)
{
    std::string findUrl = "en.wikipedia.org/wiki/";
    size_t pos = url.find(findUrl);
    if(pos == std::string::npos) {
        throw WalkerException("Must be an English Wikipedia URL");
    }

    // extract Wikipedia title
    std::string title = url.substr(pos + findUrl.length());

    //! \todo: little bobby tables?
    std::string json = grabber.grabUrl("https://en.wikipedia.org/w/api.php?action=query&format=json&prop=links&pllimit=50&plnamespace=0&titles=" + title);

    if(json != "")
    {
        Article article = getArticle(json);
        std::cout << "Article " << article.getTitle() << " has " << article.getNumLinks() << " links" << std::endl;
    }
    else
    {
        std::cerr << "Error fetching article" << std::endl;
    }
};
