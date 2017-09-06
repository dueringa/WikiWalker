//! \file WikipediaSource.h

#ifndef WIKIPEDIASOURCE_H_
#define WIKIPEDIASOURCE_H_

#include <string>
#include <vector>

#include "SourceBase.h"
#include "ArticleCollection.h"

//! use Wikipedia as article source
class WikipediaSource : public SourceBase
{
public:
  /*! add an URL to fetch
   * \param[in] url url to fetch later
   * \return whether url was added successfully
   */
  bool addUrl(std::string url);

  ArticleCollection fetch() override;

private:
  std::vector<std::string> urlsToFetch;
};

#endif  // WIKIPEDIASOURCE_H_
