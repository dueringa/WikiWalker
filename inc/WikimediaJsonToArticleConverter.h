//! \file WikimediaJsonToArticleConverter.h

#ifndef JSONTOARTICLECONVERTER_H
#define JSONTOARTICLECONVERTER_H

#include "ArticleCollection.h"

#include <memory>
#include <string>

namespace WikiWalker
{
  //! convert links in json form to article
  class WikimediaJsonToArticleConverter
  {
  public:
    //! create a new instance
    WikimediaJsonToArticleConverter() : moreData(false), continueString("")
    {
    }

    /*! convert JSON data to Article
     * convert string containing json data, which represents the links, to an
     * article.
     * \param json json data
     * \param articleCache reference to global (ew) article collection
     * \returns pointer to generated article. YOU free it!
     */
    std::shared_ptr<Article> convertToArticle(const std::string& json,
                                              ArticleCollection& articleCache);

    /*!Returns whether there's more data to fetch.
     * JSON data says there's more links to fetch.
     * \return whether there's more data to fetch.
     */
    bool hasMoreData() const
    {
      return moreData;
    }

    /*! Get the continuation data, if #hasMoreData is true
     * \return continution string
     */
    std::string getContinuationData() const
    {
      return continueString;
    }

  private:
    //! more data available
    bool moreData;
    //! string required for API operation
    std::string continueString;
  };
}
#endif /* _JSONTOARTICLECONVERTER_H */
