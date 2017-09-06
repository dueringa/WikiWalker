//! \file SourceBase.h

#ifndef SOURCEBASE_H_
#define SOURCEBASE_H_

#include "ArticleCollection.h"

//! base class for article sources
class SourceBase
{
public:
  /*! fetch articles from source
   * \returns ArticleCollection with fetched articles
   */
  virtual ArticleCollection fetch() = 0;
};

#endif  // SOURCEBASE_H_
