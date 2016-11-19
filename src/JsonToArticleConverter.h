#ifndef _JSONTOARTICLECONVERTER_H
#define _JSONTOARTICLECONVERTER_H

#include "Article.h"
#include "ArticleCollection.h"

//! convert links in json form to article
class JsonToArticleConverter
{
public:
    //! create a new instance
    JsonToArticleConverter()
        : moreData(false), continueString("") {};

    /*! convert string containing json data representing link structure to an article.
     * \param json json data
     * \param addArticle reference to global (ew) article collection
     * \returns pointer to generated article. YOU free it!
     */
    Article* convertToArticle(std::string json, ArticleCollection& addArticle);

    //! JSON data says there's mote links to fetch
    bool hasMoreData() const
    {
        return moreData;
    }

    //! continution string
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

#endif /* _JSONTOARTICLECONVERTER_H */
