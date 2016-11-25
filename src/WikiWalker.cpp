#include "WikiWalker.h"

#include <iostream>

#include "JsonToArticleConverter.h"
#include "CurlUrlCreator.h"

void WikiWalker::startWalking(std::string url)
{
    std::string apiBaseUrl = "";

    // this must be included in the URL.
    std::string domain = "wikipedia.org/";
    std::string findUrl = domain + "wiki/";
    size_t domainpos = url.find(findUrl);
    if(domainpos == std::string::npos) {
        throw WalkerException("Must be an Wikipedia URL");
    }

    // ugly URL checking
    size_t subdomainpos = url.find("http://");
    if(subdomainpos != std::string::npos) {
        if(subdomainpos != 0) {
            throw WalkerException("http:// must be at the beginning of the URL");
        }
    } else {
        subdomainpos = url.find("https://");

        if(subdomainpos != std::string::npos) {
            if(subdomainpos != 0) {
                throw WalkerException("https:// must be at the beginning of the URL");
            }
        }
        else {
            apiBaseUrl = "https://";
        }
    }

    apiBaseUrl.append(url.substr(0, domainpos + domain.length())).append("w/api.php");

    CurlUrlCreator creator(apiBaseUrl);

    // extract Wikipedia title
    std::string title = url.substr(domainpos + findUrl.length());

    creator.addParameter("action", "query").addParameter("format", "json")
            .addParameter("prop", "links").addParameter("pllimit", "max")
            .addParameter("plnamespace", "0").addParameter("formatversion", "1");
    creator.addParameter("titles", title);

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
                delete article2;
            }
        }

        std::cout << "Article " << article->getTitle() << " has " << article->getNumLinks() << " links" << std::endl;
    } else {
        std::cerr << "Error fetching article" << std::endl;
    }
}
