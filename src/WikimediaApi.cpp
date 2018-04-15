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
  /*!
   * \brief Fetches articles and converts them
   *
   * \param collection the collection to store converted articles into
   * \param creator the prepared CurlUrlCreator
   * \param grabber the CurlWikiGrabber used for fetching
   * \param continueKey the key/parameter used for continuation
   * \param generator which generator to use, if any
   */
  static void grabAndConvert(CollectionUtils::ArticleCollection& collection,
                             CurlUrlCreator& creator,
                             CurlWikiGrabber& grabber,
                             WikimediaApi::WikimediaGenerator generator)
  {
    switch(generator) {
      case WikimediaApi::WikimediaGenerator::ForwardLinkGenerator:
        creator.addParameter({{"generator", "links"},
                              {"gplnamespace", "0"},
                              {"gpllimit", "max"}});
        break;
      case WikimediaApi::WikimediaGenerator::NoGenerator:
        // nothing to do
        break;
      default:
        throw WalkerException("Unsupported generator");
        break;
    }

    std::string json = grabber.grabUrl(creator.buildUrl());
    if(!json.empty()) {
      WikimediaJsonToArticleConverter conv;
      auto conversionStatus = conv.convert(json, collection);

      while(WikimediaJsonToArticleConverter::ContinuationStatus::
                    ConversionNeedsMoreData == conversionStatus &&
            !conv.continuationData().empty()) {
        creator.addParameter(conv.continuationData());

        json             = grabber.grabUrl(creator.buildUrl());
        conversionStatus = conv.convert(json, collection);
      }
    } else {
      throw WalkerException("Error fetching article");
    }
  }

  /*!
   * \brief Gets a CurlUrlCreator with common properties
   * \param baseUrl the Wikimedia API base URL
   * \param title the article title
   * \returns the prepared CurlUrlCreator
   */
  static CurlUrlCreator getUrlCreator(std::string baseUrl, std::string title)
  {
    CurlUrlCreator creator(baseUrl);
    creator.addParameter({{"action", "query"},
                          {"format", "json"},
                          {"formatversion", "2"},
                          {"titles", title}});
    return creator;
  }

  WikimediaApi::WikimediaApi(std::string baseUrl) : baseUrl_(std::move(baseUrl))
  {
    auto result = LUrlParser::clParseURL::ParseURL(baseUrl_);
    if(!result.IsValid()) {
      throw WalkerException("Invalid URL");
    }
  }

  void WikimediaApi::fetchBackwardLinks(
      std::string title,
      WikimediaApi::WikimediaGenerator generator,
      CollectionUtils::ArticleCollection& collection)
  {
    if(title.empty()) {
      throw WalkerException("Title can't be empty.");
    }

    CurlUrlCreator creator = getUrlCreator(baseUrl_, title);
    creator.addParameter({{"prop", "linkshere"},
                          {"lhprop", "title"},
                          {"lhnamespace", "0"},
                          {"lhlimit", "max"}});

    grabAndConvert(collection, creator, grabber_, generator);
  }

  void WikimediaApi::fetchForwardLinks(
      std::string title,
      WikimediaApi::WikimediaGenerator generator,
      CollectionUtils::ArticleCollection& collection)
  {
    if(title.empty()) {
      throw WalkerException("Title can't be empty.");
    }

    CurlUrlCreator creator = getUrlCreator(baseUrl_, title);
    creator.addParameter(
        {{"prop", "links"}, {"plnamespace", "0"}, {"pllimit", "max"}});

    grabAndConvert(collection, creator, grabber_, generator);
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

      std::string path              = parsedUrl.m_Path;
      std::string pathMustStartWith = "wiki/";

      // path must begin with /wiki/
      if(!StringUtils::startsWith(path, pathMustStartWith)) {
        throw WalkerException("Must be an Wikimedia URL which uses /wiki/");
      }

      // extract Wikipedia title
      std::string title =
          path.substr(pathMustStartWith.length(),
                      path.length() - pathMustStartWith.length());

      if(title.empty()) {
        throw WalkerException("Must be an Wikimedia URL - Article missing");
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
