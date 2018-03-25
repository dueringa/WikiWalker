//! \file

#include "WikimediaApi.h"

#include <utility>

#include <LUrlParser.h>

#include "CurlUrlCreator.h"
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
          .addParameter("plnamespace", "0")
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
}  // namespace WikiWalker
