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

    //! Explicitely delete copy constructor, because curl handle
    CurlWikiGrabber(const CurlWikiGrabber& other) = delete;
    //! Explicitely delete move constructor, because curl handle
    CurlWikiGrabber(CurlWikiGrabber&& other) = delete;

    //! Explicitely delete copy assignment operator, because curl handle
    CurlWikiGrabber& operator=(const CurlWikiGrabber& other) = delete;
    //! Explicitely delete move assignment operator, because curl handle
    CurlWikiGrabber& operator=(CurlWikiGrabber&& other) = delete;

    /*! given an URL, return article JSON data
     * \param url URL of the article
     * \returns article JSON data
     */
    std::string grabUrl(const std::string& url) const;
  };
}  // namespace WikiWalker
#endif  // CURL_WIKI_GRABBER_H
