#include "CurlWikiGrabber.h"
#include <json/json.h>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    static_cast<std::string*>(userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

//! TODO: change to passing page title?
Article CurlWikiGrabber::grabUrl(std::string url)
{
    CURL *handle = curl_easy_init();

    if(NULL == handle)
    {
        throw WalkerException("error initiating curl");
    }

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "WikiWalker/ test program");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    std::string gotContent;
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &gotContent);


    gotContent = "";
    curl_easy_perform(handle);

    long httpcode = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &httpcode);

    if(httpcode != 200) {
      return Article("");
    }

    curl_easy_cleanup(handle);
    handle = NULL;

    Json::Reader reader;
    Json::Value document;
    bool success = reader.parse(gotContent, document, false);

    if(!success) {
        throw WalkerException("Error parsing JSON");
    }

    auto allPages = document.get("query", Json::Value::nullSingleton())
                            .get("pages", Json::Value::nullSingleton());

    // only get first page
    auto wantedPage = allPages.get(allPages.getMemberNames()[0],
                                   Json::Value::nullSingleton());

    Article wantedArticle(wantedPage.get("title", Json::Value::nullSingleton()).asString());

    // add links
    for(const auto &linked : wantedPage.get("links", Json::Value::nullSingleton()))
    {
        wantedArticle.addLink(
            new Article(linked.get("title", Json::Value::nullSingleton()).asString())
        );
    }

    return wantedArticle;
}

// note to self: API
// https://en.wikipedia.org/w/api.php
// /w/api.php?action=query&format=json&prop=links&plnamespace=0&titles=<title>
// maybe &pllimit=100
