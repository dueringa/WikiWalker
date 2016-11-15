#ifndef _CURL_WIKI_GRABBER_H
#define _CURL_WIKI_GRABBER_H

#include <string>
#include <curl/curl.h>
#include "WalkerException.h"
#include "Article.h"

//! Class responsible for grabbing the contents / links of an article
class CurlWikiGrabber
{
public:
    //! Create a new instance.
    //! Might throw an exception, if curl intialization failed
    CurlWikiGrabber()
    {
        int error = curl_global_init(CURL_GLOBAL_ALL);
        if(error)
        {
            throw WalkerException("CURL init failed");
        }
    }

    ~CurlWikiGrabber()
    {
        curl_global_cleanup();
    }

    //! given an URL, return an Article with its links
    Article grabUrl(std::string url);
};

#endif // _CURL_WIKI_GRABBER_H