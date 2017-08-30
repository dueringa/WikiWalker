//! \file WikiWalker.h

#ifndef WIKIWALKER_H
#define WIKIWALKER_H

#include <string>
#include "Walker.h"
#include "CurlWikiGrabber.h"

//! main "app" class
class WikiWalker : public Walker
{
public:
  /*! given an URL, start collecting links
   * \param url start point for analysis
   */
  void startWalking(const std::string& url);

  /*! Read data from cache file.
   * Used for initialization.
   * \param cache file name of the cache.
   */
  void readCache(const std::string& cacheFile);

  /*! Write data to cache file.
   * \param cache file name of the cache.
   */
  void writeCache(const std::string& cacheFile);

private:
  CurlWikiGrabber grabber;
};

#endif  // WIKIWALKER_H
