#include "CurlUrlCreator.h"

CurlUrlCreator::CurlUrlCreator(std::string baseUrl)
    : baseUrl(baseUrl)
{
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();
}

CurlUrlCreator::~CurlUrlCreator()
{
    curl_easy_cleanup(handle);
    curl_global_cleanup();
}

CurlUrlCreator& CurlUrlCreator::addParameter(std::string param, std::string value)
{
    // overwrite
    char *eval = curl_easy_escape(handle, value.c_str(), 0);
    args[param] = eval;
    curl_free(eval);
    return *this;
}

std::string CurlUrlCreator::buildUrl()
{
    std::string ret = baseUrl;
    ret.append("?");

    for(auto parpair : args) {
        ret.append(parpair.first).append("=").append(parpair.second).append("&");
    }

    ret.pop_back();

    return ret;
}

void CurlUrlCreator::reset()
{
    args.clear();
}
