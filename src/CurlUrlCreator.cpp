//! \file CurlUrlCreator.cpp

#include <utility>

#include "CurlUrlCreator.h"

namespace WikiWalker
{
  CurlUrlCreator::CurlUrlCreator(std::string baseUrl)
      : baseUrl_(std::move(baseUrl))
  {
    curl_global_init(CURL_GLOBAL_ALL);
    handle_ = curl_easy_init();
  }

  CurlUrlCreator::~CurlUrlCreator()
  {
    curl_easy_cleanup(handle_);
    curl_global_cleanup();
  }

  CurlUrlCreator& CurlUrlCreator::addParameter(const std::string& param,
                                               const std::string& value)
  {
    // overwrite
    char* eval   = curl_easy_escape(handle_, value.c_str(), 0);
    args_[param] = eval;
    curl_free(eval);
    return *this;
  }

  CurlUrlCreator& CurlUrlCreator::addParameter(
      const std::map<const std::string, const std::string>& params)
  {
    /* we need to escape the values, so doing a direct ranged insert isn't
     * possible */
    for(auto& paramterPair : params) {
      addParameter(paramterPair.first, paramterPair.second);
    }
    return *this;
  }

  std::string CurlUrlCreator::buildUrl() const
  {
    std::string ret = baseUrl_;
    ret.append("?");

    for(auto parpair : args_) {
      ret.append(parpair.first).append("=").append(parpair.second).append("&");
    }

    ret.pop_back();

    return ret;
  }

  void CurlUrlCreator::reset()
  {
    args_.clear();
  }
}  // namespace WikiWalker
