//! \file WikimediaJsonToArticleConverter.h

#ifndef WIKIWALKER_WIKIMEDIAJSONTOARTICLECONVERTER_H
#define WIKIWALKER_WIKIMEDIAJSONTOARTICLECONVERTER_H

#include "ArticleCollection.h"

#include <memory>
#include <string>

namespace WikiWalker
{
  /*! converts a JSON reply from the Wikimedia query links request.
   * This class only supports JSON format 2! JSON format 1 is not supported
   * anymore.
   */
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
    WikimediaJsonToArticleConverter() : continueString_("")
    {
    }

    /*! convert JSON data to Article.
     * convert a string containing json data containing the reply of a Wikimedia
     * query links request in JSON format 2 to an article with links and puts it
     * in an ArticleCollection.
     * \param json json data
     * \param articleCache reference to an article collection that gets filled
     * \returns the conversion status. If this is
     * #ContinuationStatus::ConversionNeedsMoreData, get continuation data with
     * #continuationData.
     */
    ContinuationStatus convert(
        const std::string& json,
        CollectionUtils::ArticleCollection& articleCache);

    /*! Get the continuation data, if
     * #ContinuationStat::ConversionNeedsMoreData was returned by
     * #convert.
     * \return continuation string
     */
    std::string continuationData() const
    {
      return continueString_;
    }

  private:
    //! string required for API operation
    std::string continueString_;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_WIKIMEDIAJSONTOARTICLECONVERTER_H
