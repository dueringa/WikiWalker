//! \file WikiWalker.cpp

#include "WikiWalker.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "LUrlParser.h"

#include "JsonSerializer.h"
#include "StringUtils.h"
#include "WalkerException.h"
#include "WikimediaApi.h"

// since the class is names like the namespace, this is a bit awkward...

namespace WikiWalker
{
  void WikiWalker::start(const std::string& url)
  {
    auto info = WikimediaApiUtils::parseArticleUrl(url);
    WikimediaApi wapi(info.apiBaseUrl);
    wapi.fetchForwardLinks(info.articleTitle, fetchGenerator_, articleSet_);
  }

  void WikiWalker::readCache(const std::string& cacheFile)
  {
    JsonSerializer jser;
    std::ifstream cache(cacheFile);

    // assumption: having write-only access to a file is so rare that I don't
    // care also, currently the file is used for both read and write, so
    // initially it won't exist.
    if(!cache.is_open()) {
      return;
    }

    jser.deserialize(articleSet_, cache);

    // doesn't work anymore since jsoncpp class changed
    // assert(cache.eof());

    if(cache.fail()) {
      cache.close();
      throw WalkerException("Error reading from file");
    }
  }

  void WikiWalker::writeCache(const std::string& cacheFile)
  {
    JsonSerializer w;

    std::ofstream cache(cacheFile, std::ios::trunc);

    if(!cache.is_open()) {
      throw WalkerException("Error writing to cache file. Check permissions.");
    }

    w.serialize(articleSet_, cache);

    if(cache.fail() || cache.bad()) {
      cache.close();
      throw WalkerException("I/O eception when writing to cache file");
    }

    cache.flush();
    cache.close();
  }
}  // namespace WikiWalker
