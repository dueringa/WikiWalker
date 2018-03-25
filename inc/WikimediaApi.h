//! \file

#ifndef WIKIWALKER_WIKIMEDIAAPI_H
#define WIKIWALKER_WIKIMEDIAAPI_H

#include <string>

#include "ArticleCollection.h"
#include "CurlWikiGrabber.h"

namespace WikiWalker
{
  /*! \brief Represents the Wikimedia API.
   * \details the needed parts.
   * The API already converts the string/Json data to a
   * CollectionUtils::ArticleCollection.
   */
  class WikimediaApi
  {
  public:
    /*! \brief Create a new instance of the WikimediaApi
     * \param baseUrl the Base URL of the Wikimedia instance, including the
     * protocol \todo Take "DataFetcher" or whatever to replace default CURL
     */
    explicit WikimediaApi(std::string baseUrl);

    /*! Get the forward links from a specified title and store it in the
     * collection.
     *
     * \param title the title of the WikimediaArticle.
     * \param alsoUseGenetator whether to use a generator to also fetch links in
     * articles linked from the specified artitle.
     * \param[out] the collection to store into. If article always exists in
     * collection, it will be skipped (TODO: REALLY?).
     */
    void fetchForwardLinks(std::string title,
                           bool alsoUseGenetator,
                           CollectionUtils::ArticleCollection& collection);

  private:
    std::string baseUrl_;
    CurlWikiGrabber grabber_;
  };
}  // namespace WikiWalker

#endif  // WIKIWALKER_WIKIMEDIAAPI_H
