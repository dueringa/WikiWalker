#include "WikiWalker.h"

#include <iostream>

#include "JsonToArticleConverter.h"
#include "CurlUrlCreator.h"

void WikiWalker::startWalking(std::string url)
{
    std::string findUrl = "en.wikipedia.org/wiki/";
    size_t pos = url.find(findUrl);
    if(pos == std::string::npos) {
        throw WalkerException("Must be an English Wikipedia URL");
    }

    std::string apiBaseUrl = "https://en.wikipedia.org/w/api.php";
    CurlUrlCreator creator(apiBaseUrl);


    // extract Wikipedia title
    std::string title = url.substr(pos + findUrl.length());

    creator.addParameter("action", "query").addParameter("format", "json")
            .addParameter("prop", "links").addParameter("pllimit", "max")
            .addParameter("plnamespace", "0").addParameter("formatversion", "1");
    creator.addParameter("titles", title);

    //! \todo: little bobby tables?
    std::string json = grabber.grabUrl(creator.buildUrl());

    if(json != "") {
        JsonToArticleConverter conv;
        Article* article = conv.convertToArticle(json, articleSet);

        while(conv.hasMoreData() && conv.getContinuationData() != "") {
            creator.addParameter("plcontinue", conv.getContinuationData());

            json = grabber.grabUrl(creator.buildUrl());
            Article* article2 = conv.convertToArticle(json, articleSet);

            if(article != article2) {
                for(auto x = article2->linkBegin(); x != article2->linkEnd(); x++) {
                    article->addLink(*x);
                }

                // delete duplicate article
                //! \todo cleanup problem: linked articles may also already exist in collection -> leak
                delete article2;
            }
        }

        std::cout << "Article " << article->getTitle() << " has " << article->getNumLinks() << " links" << std::endl;
    } else {
        std::cerr << "Error fetching article" << std::endl;
    }
}
