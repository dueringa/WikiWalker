//! \file WikiWalker.cpp

#include "WikiWalker.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "Article.h"
#include "CacheJsonToArticleConverter.h"
#include "CurlUrlCreator.h"
#include "JsonCacheSource.h"
#include "ToJsonWriter.h"
#include "WalkerException.h"
#include "WikimediaJsonToArticleConverter.h"

void WikiWalker::startWalking(const std::string& url)
{
  std::string apiBaseUrl;

  // this must be included in the URL.
  std::string domain  = "wikipedia.org/";
  std::string findUrl = domain + "wiki/";
  size_t domainpos    = url.find(findUrl);

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
    } else {
      apiBaseUrl = "https://";
    }
  }

  apiBaseUrl.append(url.substr(0, domainpos + domain.length()))
      .append("w/api.php");

  CurlUrlCreator creator(apiBaseUrl);

  // extract Wikipedia title
  std::string title = url.substr(domainpos + findUrl.length());

  creator.addParameter("action", "query")
      .addParameter("format", "json")
      .addParameter("prop", "links")
      .addParameter("pllimit", "max")
      .addParameter("plnamespace", "0")
      .addParameter("formatversion", "1");
  creator.addParameter("titles", title);

  std::string json = grabber.grabUrl(creator.buildUrl());

  if(json != "") {
    WikimediaJsonToArticleConverter conv;
    auto article = conv.convertToArticle(json, articleSet);

    while(conv.hasMoreData() && conv.getContinuationData() != "") {
      creator.addParameter("plcontinue", conv.getContinuationData());

      json          = grabber.grabUrl(creator.buildUrl());
      auto article2 = conv.convertToArticle(json, articleSet);

      if(article != article2) {
        for(auto x = article2->linkBegin(); x != article2->linkEnd(); x++) {
          article->addLink(*x);
        }
      }
    }

    std::cout << "Article " << article->getTitle() << " has "
              << article->getNumLinks() << " links" << std::endl;
  } else {
    std::cerr << "Error fetching article" << std::endl;
  }
}

void WikiWalker::readCache(const std::string& cacheFile)
{
  JsonCacheSource ac(cacheFile);
  articleSet.merge(ac.fetch(), ArticleCollection::MergeStrategy::IgnoreDuplicates);
}

void WikiWalker::writeCache(const std::string& cacheFile)
{
  ToJsonWriter w;

  std::ofstream cache(cacheFile, std::ios::trunc);

  if(!cache.is_open()) {
    throw WalkerException("Error writing to cache file. Check permissions.");
  }

  w.output(articleSet, cache);

  if(cache.fail() || cache.bad()) {
    cache.close();
    throw WalkerException("I/O eception when writing to cache file");
  }

  cache.flush();
  cache.close();
}
