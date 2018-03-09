//! \file WikiWalker.cpp

#include "WikiWalker.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "LUrlParser.h"

#include "Article.h"
#include "CacheJsonToArticleConverter.h"
#include "CurlUrlCreator.h"
#include "StringUtils.h"
#include "ToJsonWriter.h"
#include "WalkerException.h"
#include "WikimediaJsonToArticleConverter.h"

// since the class is names like the namespace, this is a bit awkward...

void WikiWalker::WikiWalker::startWalking(const std::string& url)
{
  // try parsing URL
  auto parsedUrl = LUrlParser::clParseURL::ParseURL(url);
  if(!parsedUrl.IsValid()) {
    // if URL with no protocol is passed, use HTTPS
    std::string protocol = "https://";
    parsedUrl            = LUrlParser::clParseURL::ParseURL(protocol + url);

    if(!parsedUrl.IsValid()) {
      throw WalkerException("Invalid URL");
    }
  }

  size_t domainpos              = parsedUrl.m_Host.find("wikipedia.org");
  std::string path              = parsedUrl.m_Path;
  std::string pathMustStartWith = "wiki/";

  // Host must contain wikipedia.org, path must begin with /wiki/
  if(domainpos == std::string::npos ||
     !StringUtils::startsWith(path, pathMustStartWith)) {
    throw WalkerException("Must be an Wikipedia URL");
  }
  // extract Wikipedia title
  std::string title = path.substr(pathMustStartWith.length(),
                                  path.length() - pathMustStartWith.length());

  if(title.empty()) {
    throw WalkerException("Must be an Wikipedia URL - Article missing");
  }

  std::string apiBaseUrl;

  apiBaseUrl = parsedUrl.m_Scheme;
  apiBaseUrl.append("://");
  apiBaseUrl.append(parsedUrl.m_Host);
  apiBaseUrl.append("/w/api.php");

  CurlUrlCreator creator(apiBaseUrl);

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

void WikiWalker::WikiWalker::readCache(const std::string& cacheFile)
{
  CacheJsonToArticleConverter cjta;
  std::ifstream cache(cacheFile);

  // assumption: having write-only access to a file is so rare that I don't care
  // also, currently the file is used for both read and write, so initially it
  // won't exist.
  if(!cache.is_open()) {
    return;
  }

  cjta.convertToArticle(cache, articleSet);

  assert(cache.eof());

  if(cache.fail()) {
    cache.close();
    throw WalkerException("Error reading from file");
  }
}

void WikiWalker::WikiWalker::writeCache(const std::string& cacheFile)
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
