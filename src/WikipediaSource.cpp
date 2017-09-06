//! \file WikipediaSource.cpp

#include "WikipediaSource.h"

#include "WalkerException.h"

void fetchUrl(std::string url);
bool checkUrl(std::string url);

bool WikipediaSource::addUrl(std::string url)
{
  if(checkUrl(url)) {
    urlsToFetch.push_back(url);
    return true;
  }
  return false;
}

ArticleCollection WikipediaSource::fetch()
{
  ArticleCollection ac;

  for(const auto& url : urlsToFetch) {
    fetchUrl(url);
  }

  return ac;
}

bool checkUrl(std::string url)
{
  size_t domainpos = url.find("wikipedia.org/wiki/");
  return domainpos != std::string::npos;
}

void fetchUrl(std::string url)
{
}
