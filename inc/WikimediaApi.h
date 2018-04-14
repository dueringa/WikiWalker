//! \file

#ifndef WIKIWALKER_WIKIMEDIAAPI_H
#define WIKIWALKER_WIKIMEDIAAPI_H

#include <string>

#include "ArticleCollection.h"
#include "CurlWikiGrabber.h"

namespace WikiWalker
{
  namespace WikimediaApiUtils
  {
    //! Represents URL components
    struct WikimediaUrlInfo {
      //! the API base URL
      std::string apiBaseUrl;
      //! the article title
      std::string articleTitle;
    };

    /*! Parses a Wikimedia article URL and returns components.
     * \param articleUrl the Wikimedia article URL.
     * \returns a stuct containing base URL and title.
     */
    WikimediaUrlInfo parseArticleUrl(const std::string& articleUrl);
  }  // namespace WikimediaApiUtils

  /*! \brief Represents the Wikimedia API.
   * \details the needed parts.
   * The API already converts the string/Json data to a
   * CollectionUtils::ArticleCollection.
   */
  class WikimediaApi
  {
  public:
    //! Usable generators
    enum class WikimediaGenerator {
      //! Don't use a generator
      NoGenerator,
      //! use the forwardlinks generator
      ForwardLinkGenerator
    };

    /*! \brief Create a new instance of the WikimediaApi
     * \param baseUrl the API base URL of the Wikimedia instance, including the
     * protocol.
     * \todo Take "DataFetcher" or whatever to replace default CURL
     */
    explicit WikimediaApi(std::string baseUrl);

    /*! Get the forward links from a specified title and store it in the
     * collection.
     *
     * \param title the title of the WikimediaArticle.
     * \param generator which generator to use to also fetch links in
     * articles linked from the specified artitle.
     * \param[out] collection the collection to store into. If article already
     * exists in collection, it will be skipped (TODO: REALLY?).
     */
    void fetchForwardLinks(std::string title,
                           WikimediaGenerator generator,
                           CollectionUtils::ArticleCollection& collection);

    /*! Get the backward links from a specified title and store it in the
     * collection.
     *
     * \param title the title of the WikimediaArticle.
     * \param generator which generator to use to also fetch links in
     * articles linked from the specified artitle.
     * \param[out] collection the collection to store into. If article already
     * exists in collection, it will be skipped (TODO: REALLY?).
     */
    void fetchBackwardLinks(std::string title,
                            WikimediaGenerator generator,
                            CollectionUtils::ArticleCollection& collection);

  private:
    std::string baseUrl_;
    CurlWikiGrabber grabber_;
  };
}  // namespace WikiWalker

#endif  // WIKIWALKER_WIKIMEDIAAPI_H
