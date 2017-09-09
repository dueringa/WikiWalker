//! \file JsonCacheSource.h

#ifndef JSONCACHESOURCE_H_
#define JSONCACHESOURCE_H_

#include <string>
#include "SourceBase.h"

//! source that can read from a json cache file
class JsonCacheSource : public SourceBase
{
public:
  /*! create a new JsonCacheSource reading from a specified file
   * \param pathToFile path to json cache file.
   * \detail no file checking done in constructor. Only in fetch.
   */
  JsonCacheSource(std::string pathToFile);

  /*! read data from file specified in constructor.
   * \returns ArticleCollection created from articles in file
   * \details if file doesn't exist, or can't read from it, returns empty
   * ArticleCollection
   */
  ArticleCollection fetch() override;

private:
  std::string fileName;
};

#endif  // JSONCACHESOURCE_H_
