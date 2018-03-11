//! \file CurlUrlCreator.cpp

#include <utility>

#include "CurlUrlCreator.h"

namespace WikiWalker
{
  CurlUrlCreator::CurlUrlCreator(std::string baseUrl) : _baseUrl(std::move(baseUrl))
  {
    curl_global_init(CURL_GLOBAL_ALL);
    handle = curl_easy_init();
  }

  CurlUrlCreator::~CurlUrlCreator()
  {
    curl_easy_cleanup(handle);
    curl_global_cleanup();
  }

  CurlUrlCreator& CurlUrlCreator::addParameter(const std::string& param,
                                               const std::string& value)
  {
    // overwrite
    char* eval  = curl_easy_escape(handle, value.c_str(), 0);
    args[param] = eval;
    curl_free(eval);
    return *this;
  }

  std::string CurlUrlCreator::buildUrl() const
  {
    std::string ret = _baseUrl;
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
}  // namespace WikiWalker