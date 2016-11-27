//! \file CacheJsonToArticleConverter.h

#ifndef _CACHEJSONTOARTICLECONVERTER_H
#define _CACHEJSONTOARTICLECONVERTER_H

#include "ArticleCollection.h"

/*! Read Article from JSON cache file.
 * This class does the opposite to #ToJsonWriter.
 */
class CacheJsonToArticleConverter
{
public:
    /*! Convert JSON data from cache file into an article collection.
     * \param json JSON data
     * \param articleCache Article collection to add articles to. Will
     *          also look up existing articles there.
     * \returns reference to passed article collection.
     */
    ArticleCollection& convertToArticle(std::string json, ArticleCollection& articleCache);
};

#endif // _CACHEJSONTOARTICLECONVERTER_H
