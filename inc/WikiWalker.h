//! \file WikiWalker.h

#ifndef WIKIWALKER_H
#define WIKIWALKER_H

#include <string>

#include "CurlWikiGrabber.h"
#include "Walker.h"

namespace WikiWalker
{
  //! main "app" class
  class WikiWalker : public Walker
  {
  public:
    //! Creates a new instance
    WikiWalker() : Walker(), fetchGenerator(false), grabber()
    {
    }

    /*! given an URL, start collecting links
     * \param url start point for analysis
     */
    void startWalking(const std::string& url);

    /*! fetch not only requested article, but also linked ones.
     * \param depth whether to fetch linked articles as well
     */
    void setDeep(bool depth)
    {
      fetchGenerator = depth;
    }

    /*! Read data from cache file.
     * Used for initialization.
     * \param cacheFile file name of the cache.
     */
    void readCache(const std::string& cacheFile);

    /*! Write data to cache file.
     * \param cacheFile file name of the cache.
     */
    void writeCache(const std::string& cacheFile);

    //! \see CurlWikiGrabber::skipSslVerification
    void skipSslVerification(bool state)
    {
      grabber.skipSslVerification(state);
    }

  private:
    CurlWikiGrabber grabber;
    bool fetchGenerator;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_H
