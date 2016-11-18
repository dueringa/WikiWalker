#include "WikiWalker.h"
#include "WikiWalker.h"
#include "JsonToArticleConverter.h"

#include <iostream>
#include <json/json.h>

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
    std::string json = grabber.grabUrl(title);

    if(json != "")
    {
        JsonToArticleConverter conv;
        Article* article = conv.convertToArticle(json, articleSet);

        while(conv.hasMoreData() && conv.getContinuationData() != "") {
            std::string json = grabber.grabUrl(title, conv.getContinuationData());
            Article* article2 = conv.convertToArticle(json, articleSet);

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
