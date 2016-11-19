#ifndef _ARTICLE_COLLECTION_H
#define _ARTICLE_COLLECTION_H

#include <map>

#include "Article.h"

/*! Collection of available articles.
 *  This should be used as "cache".
 */
class ArticleCollection
{
public:
    /*! add article to collection.
     * \param article article to add
     * \return true if insertion took place
     *          false if it failed (e.g. another article with the same title already exists
     */
    bool add(Article* article)
    {
        auto ret = articleSet.insert(std::make_pair(article->getTitle(), article));
        return ret.second;
    }

    //! get number of articles in collection
    size_t getNumArticles() const
    {
        return articleSet.size();
    }

    ~ArticleCollection();

private:
    // we need to avoid duplicate article instances.
    // we do this by associating an article title with its instance
    std::map<std::string, Article*> articleSet;
};

#endif // _ARTICLE_COLLECTION_H
