//! \file CurlWikiGrabber.cpp

#include "CurlWikiGrabber.h"

#include <cassert>

#include <curl/curl.h>

#include "WalkerException.h"

static size_t write_callback(char* ptr,
                             size_t size,
                             size_t nmemb,
                             void* userdata)
{
  static_cast<std::string*>(userdata)->append(ptr, size * nmemb);
  return size * nmemb;
}

CurlWikiGrabber::CurlWikiGrabber()
{
  int error = curl_global_init(CURL_GLOBAL_ALL);

  if(error != 0) {
    throw WalkerException("CURL init failed");
  }
}

CurlWikiGrabber::~CurlWikiGrabber()
{
  curl_global_cleanup();
}

//! \todo Curl return code checking
std::string CurlWikiGrabber::grabUrl(const std::string& url) const
{
  CURL* handle = curl_easy_init();

  if(nullptr == handle) {
    throw WalkerException("error initiating curl");
  }

  CURLcode crv = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  assert(crv == 0);
  crv = curl_easy_setopt(handle, CURLOPT_USERAGENT, "WikiWalker/ test program");
  assert(crv == 0);
  crv = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
  assert(crv == 0);

  std::string gotContent;
  crv = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &gotContent);
  assert(crv == 0);

  gotContent = "";
  crv        = curl_easy_perform(handle);

  if(crv != 0) {
    const char* err = curl_easy_strerror(crv);
    std::string text(err);
    throw WalkerException(text);
  }

  long httpcode = 0;
  crv           = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &httpcode);
  assert(crv == 0);

  curl_easy_cleanup(handle);

  handle = nullptr;

  if(httpcode != 200) {
    return "";
  }

  return gotContent;
}

// note to self: API
// https://en.wikipedia.org/w/api.php
// /w/api.php?action=query&format=json&prop=links&plnamespace=0&titles=<title>
// maybe &pllimit=100
