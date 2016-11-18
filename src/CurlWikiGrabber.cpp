#include "CurlWikiGrabber.h"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    static_cast<std::string*>(userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

CurlWikiGrabber::CurlWikiGrabber()
{
    int error = curl_global_init(CURL_GLOBAL_ALL);
    if(error)
    {
        throw WalkerException("CURL init failed");
    }
}

//! \todo change to passing page title?
//! \todo Curl return code checking
std::string CurlWikiGrabber::grabUrl(std::string title, const std::string& continuationData) const
{
    CURL *handle = curl_easy_init();

    if(NULL == handle)
    {
        throw WalkerException("error initiating curl");
    }

    char* titleEncoded = curl_easy_escape(handle, title.c_str(), 0);
    std::string apiBaseUrl = "https://en.wikipedia.org/w/api.php";
    std::string url = apiBaseUrl.append("?action=query&format=json&prop=links&pllimit=50&plnamespace=0&formatversion=1");

    if(!continuationData.empty()) {
        url = apiBaseUrl.append("&plcontinue=").append(continuationData);
    }

    url = apiBaseUrl.append("&titles=").append(titleEncoded);

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "WikiWalker/ test program");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    std::string gotContent;
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &gotContent);


    gotContent = "";
    curl_easy_perform(handle);

    long httpcode = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &httpcode);

    curl_easy_cleanup(handle);
    handle = NULL;

    if(httpcode != 200) {
      return "";
    }

    return gotContent;
}

// note to self: API
// https://en.wikipedia.org/w/api.php
// /w/api.php?action=query&format=json&prop=links&plnamespace=0&titles=<title>
// maybe &pllimit=100
