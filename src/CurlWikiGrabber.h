#ifndef _CURL_WIKI_GRABBER_H
#define _CURL_WIKI_GRABBER_H

#include <string>
#include <curl/curl.h>
#include "WalkerException.h"

class CurlWikiGrabber
{
public:
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

    std::string grabUrl(std::string url);
};

#endif // _CURL_WIKI_GRABBER_H
