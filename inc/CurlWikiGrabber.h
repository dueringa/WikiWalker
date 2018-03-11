//! \file CurlWikiGrabber.h

#ifndef CURL_WIKI_GRABBER_H
#define CURL_WIKI_GRABBER_H

#include <string>

namespace WikiWalker
{
  //! Class responsible for grabbing the contents / links of an article
  class CurlWikiGrabber
  {
  public:
    /*! Create a new instance.
     * Might throw an exception, if curl intialization failed
     */
    CurlWikiGrabber();

    ~CurlWikiGrabber();

    /*! given an URL, return article JSON data
     * \param url URL of the article
     * \returns article JSON data
     */
    std::string grabUrl(const std::string& url) const;
  };
}  // namespace WikiWalker
#endif  // CURL_WIKI_GRABBER_H
