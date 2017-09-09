#include "JsonCacheSource.h"

#include <cassert>
#include <fstream>
#include <utility>

#include "CacheJsonToArticleConverter.h"
#include "WalkerException.h"

JsonCacheSource::JsonCacheSource(std::string pathToFile)
    : fileName(std::move(pathToFile))
{
}

ArticleCollection JsonCacheSource::fetch()
{
  std::ifstream file(fileName);
  if(!file) {
    return ArticleCollection();
  }

  CacheJsonToArticleConverter cjta;
  std::string json;

  //! \todo what happens with megabyte-big data? looks like str.max_size is the
  //! limit for reading?
  std::getline(file, json);

  assert(file.eof());

  if(file.fail()) {
    file.close();
    throw WalkerException("Error reading from file");
  }

  ArticleCollection ac;
  cjta.convertToArticle(json, ac);
  return ac;
}
