#include "CurlWikiGrabber.h"
#include <json/json.h>

//! Todo: look for better implementation
static std::string gotContent = "";
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    gotContent.append(ptr, size * nmemb);
    return size * nmemb;
}

//! TODO: change to passing page title?
std::string CurlWikiGrabber::grabUrl(std::string url)
{
    CURL *handle = curl_easy_init();

    if(NULL == handle)
    {
        throw WalkerException("error initiating curl");
    }

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "WikiWalker/ test program");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    gotContent = "";
    curl_easy_perform(handle);

    curl_easy_cleanup(handle);
    handle = NULL;

    Json::Reader reader;
    Json::Value document;
    bool success = reader.parse(gotContent, document, false);

    if(!success) {
        throw WalkerException("Error parsing JSON");
    }

    return std::string(gotContent);
}

// note to self: API
// https://en.wikipedia.org/w/api.php
// /w/api.php?action=query&format=json&prop=links&plnamespace=0&titles=<title>
// maybe &pllimit=100
