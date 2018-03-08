//! \file CacheJsonToArticleConverter.h

#ifndef CACHEJSONTOARTICLECONVERTER_H
#define CACHEJSONTOARTICLECONVERTER_H

#include <istream>
#include <string>

#include "ArticleCollection.h"

namespace WikiWalker
{
  /*! Read Article from JSON cache file.
   * This class does the opposite to #ToJsonWriter.
   */
  class CacheJsonToArticleConverter
  {
  public:
    /*! Convert JSON data from a string into an article collection.
     * \param json JSON data in a string
     * \param articleCache Article collection to add articles to. Will
     *          also look up existing articles there.
     * \returns reference to passed article collection.
     */
    ArticleCollection& convertToArticle(const std::string& json,
                                        ArticleCollection& articleCache);

    /*! Convert JSON data from a string into an article collection.
     * \param json stream with JSON data
     * \param articleCache Article collection to add articles to. Will
     *          also look up existing articles there.
     * \returns reference to passed article collection.
     */
    ArticleCollection& convertToArticle(std::istream& json,
                                        ArticleCollection& articleCache);
  };
}
#endif  // CACHEJSONTOARTICLECONVERTER_H
