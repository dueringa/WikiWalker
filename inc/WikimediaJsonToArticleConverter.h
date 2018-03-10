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
    //! Represents the state of the json to article conversion
    enum class ContinuationStatus {
      //! the conversion is completed
      ConversionCompleted,
      //! the conversion indicated more data is needed
      ConversionNeedsMoreData
    };

    //! create a new instance
    WikimediaJsonToArticleConverter() : continueString("")
    {
    }

    /*! convert JSON data to Article
     * convert string containing json data, which represents the links, to an
     * article.
     * \param json json data
     * \param articleCache reference to global (ew) article collection
     * \returns the conversion status. If this is
     * #ContinuationStatus::ConversionNeedsMoreData, get continuation data with
     * #getContinuationData.
     */
    ContinuationStatus convertToArticle(const std::string& json,
                                        ArticleCollection& articleCache);

    /*! Get the continuation data, if
     * #ContinuationStat::ConversionNeedsMoreData was returned by
     * #convertToArticle.
     * \return continuation string
     */
    std::string getContinuationData() const
    {
      return continueString;
    }

  private:
    //! string required for API operation
    std::string continueString;
  };
}
#endif /* _JSONTOARTICLECONVERTER_H */
