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

  ArticleCollection fetch() override;
};

#endif  // JSONCACHESOURCE_H_
