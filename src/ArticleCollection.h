#ifndef _ARTICLE_COLLECTION_H
#define _ARTICLE_COLLECTION_H

#include <set>

#include "Article.h"

class ArticleCollection
{
public:
    void add(Article* article)
    {
        articleSet.insert(article);
    }

    size_t getNumArticles() const
    {
        return articleSet.size();
    }

private:
    struct article_compare {
        bool operator()(const Article* lhs, const Article* rhs) const
        {
            // use title to compare
            return lhs->getTitle().compare(rhs->getTitle()) < 0;
        }
    };

    // we need to avoid duplicate article instances.
    // we do this by associating an article title with its instance
    std::set<Article*, article_compare> articleSet;
};

#endif // _ARTICLE_COLLECTION_H