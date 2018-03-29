//! \file

#include "WikimediaApi.h"

#include <utility>

#include <LUrlParser.h>

#include "CurlUrlCreator.h"
#include "StringUtils.h"
#include "WalkerException.h"
#include "WikimediaJsonToArticleConverter.h"

namespace WikiWalker
{
  WikimediaApi::WikimediaApi(std::string baseUrl) : baseUrl_(std::move(baseUrl))
  {
    auto result = LUrlParser::clParseURL::ParseURL(baseUrl_);
    if(!result.IsValid()) {
      throw WalkerException("Invalid URL");
    }
  }

  void WikimediaApi::fetchForwardLinks(
      std::string title,
      bool alsoUseGenetator,
      CollectionUtils::ArticleCollection& collection)
  {
    if(title.empty()) {
      throw WalkerException("Title can't be empty.");
    }

    CurlUrlCreator creator(baseUrl_);

    creator.addParameter("action", "query")
        .addParameter("format", "json")
        .addParameter("prop", "links")
        .addParameter("pllimit", "max")
        .addParameter("plnamespace", "0")
        .addParameter("formatversion", "2");
    creator.addParameter("titles", title);

    std::string json = grabber_.grabUrl(creator.buildUrl());
    if(!json.empty()) {
      WikimediaJsonToArticleConverter conv;
      auto conversionStatus = conv.convert(json, collection);

      while(WikimediaJsonToArticleConverter::ContinuationStatus::
                    ConversionNeedsMoreData == conversionStatus &&
            !conv.continuationData().empty()) {
        creator.addParameter("plcontinue", conv.continuationData());

        json             = grabber_.grabUrl(creator.buildUrl());
        conversionStatus = conv.convert(json, collection);
      }
    } else {
      throw WalkerException("Error fetching article");
    }

    if(alsoUseGenetator) {
      creator.addParameter("generator", "links")
          .addParameter("gplnamespace", "0")
          .addParameter("gpllimit", "max");
      json = grabber_.grabUrl(creator.buildUrl());

      if(!json.empty()) {
        WikimediaJsonToArticleConverter conv;
        auto conversionStatus = conv.convert(json, collection);

        while(WikimediaJsonToArticleConverter::ContinuationStatus::
                      ConversionNeedsMoreData == conversionStatus &&
              !conv.continuationData().empty()) {
          creator.addParameter("plcontinue", conv.continuationData());

          json             = grabber_.grabUrl(creator.buildUrl());
          conversionStatus = conv.convert(json, collection);
        }
      }
    }
  }

  namespace WikimediaApiUtils
  {
    WikimediaUrlInfo parseArticleUrl(const std::string& articleUrl)
    {
      WikimediaUrlInfo urlInfo;

      // try parsing URL
      auto parsedUrl = LUrlParser::clParseURL::ParseURL(articleUrl);
      if(!parsedUrl.IsValid()) {
        // if URL with no protocol is passed, use HTTPS
        std::string protocol = "https://";
        parsedUrl = LUrlParser::clParseURL::ParseURL(protocol + articleUrl);

        if(!parsedUrl.IsValid()) {
          throw WalkerException("Invalid URL");
        }
      }

      size_t domainpos              = parsedUrl.m_Host.find("wikipedia.org");
      std::string path              = parsedUrl.m_Path;
      std::string pathMustStartWith = "wiki/";

      // Host must contain wikipedia.org, path must begin with /wiki/
      if(domainpos == std::string::npos ||
         !StringUtils::startsWith(path, pathMustStartWith)) {
        throw WalkerException("Must be an Wikipedia URL");
      }

      // extract Wikipedia title
      std::string title =
          path.substr(pathMustStartWith.length(),
                      path.length() - pathMustStartWith.length());

      if(title.empty()) {
        throw WalkerException("Must be an Wikipedia URL - Article missing");
      }

      std::string apiBaseUrl;

      apiBaseUrl = parsedUrl.m_Scheme;
      apiBaseUrl.append("://");
      apiBaseUrl.append(parsedUrl.m_Host);
      apiBaseUrl.append("/w/api.php");

      urlInfo.articleTitle = std::move(title);
      urlInfo.apiBaseUrl   = std::move(apiBaseUrl);

      return urlInfo;
    }
  }  // namespace WikimediaApiUtils
}  // namespace WikiWalker
