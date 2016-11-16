#include "WikiWalker.h"

#include <iostream>
#include <json/json.h>

//! represents continue operations
struct ContinueData
{
    //! Creates a new ContinueData object
    ContinueData() : moreData(false), continueString("") {};
    //! more data available
    bool moreData;
    //! string required for API operation
    std::string continueString;
};

//! \todo really ugly workaround, passing in the ArticleCollection instance... :/
Article* getArticle(std::string json, ContinueData &contData, ArticleCollection* addArticle)
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

    Article* wantedArticle = new Article(wantedPage.get("title", Json::Value::nullSingleton()).asString());
    addArticle->add(wantedArticle);

    // add links
    for(const auto &linked : wantedPage.get("links", Json::Value::nullSingleton()))
    {
        auto par = new Article(linked.get("title", Json::Value::nullSingleton()).asString());
        wantedArticle->addLink(
            par
        );
        addArticle->add(par);
    }

    if(!document.isMember("batchcomplete")) {
        contData.moreData = true;
        contData.continueString =
          document.get("continue", Json::Value::nullSingleton())
                  .get("plcontinue", Json::Value::nullSingleton())
                    .asString();
    }
    else
    {
        contData.moreData = false;
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
    std::string json = grabber.grabUrl("https://en.wikipedia.org/w/api.php?action=query&format=json&prop=links&pllimit=50&plnamespace=0&formatversion=1&titles=" + title);

    if(json != "")
    {
        ContinueData contData;
        Article* article = getArticle(json, contData, &articleSet);

        while(contData.moreData && contData.continueString != "") {
            std::string json = grabber.grabUrl("https://en.wikipedia.org/w/api.php?action=query&format=json&prop=links&pllimit=50&plnamespace=0&formatversion=1&plcontinue=" + contData.continueString + "&titles=" + title);
            Article* article2 = getArticle(json, contData, &articleSet);

            for(auto x = article2->linkBegin(); x != article2->linkEnd(); x++) {
                article->addLink(*x);
            }
            // delete duplicate article
            //! \todo cleanup problem: linked articles may also already exist in collection -> leak
            delete article2;
        }

        std::cout << "Article " << article->getTitle() << " has " << article->getNumLinks() << " links" << std::endl;
    }
    else
    {
        std::cerr << "Error fetching article" << std::endl;
    }
}
