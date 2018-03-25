//! \file

#ifndef WIKIWALKER_WIKIMEDIAAPI_H
#define WIKIWALKER_WIKIMEDIAAPI_H

#include <string>
#include <utility>

namespace WikiWalker
{
  class WikimediaApi
  {
  public:
    explicit WikimediaApi(std::string baseUrl) : baseUrl_(std::move(baseUrl))
    {
    }

  private:
    std::string baseUrl_;
  };
}  // namespace WikiWalker

#endif  // WIKIWALKER_WIKIMEDIAAPI_H
