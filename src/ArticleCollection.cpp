//! \file ArticleCollection.cpp

#include "ArticleCollection.h"

#include "Article.h"

ArticleCollection::~ArticleCollection()
{
    /* this is still kinda ugly, since other pointers may exist...
     * also, deleting stack-created articles is going to get ugly
     * (although you shouldn't have pointers to stack variables
     * anyway... */
    for(auto it = articleSet.rbegin(); it != articleSet.rend(); it++) {
        /* reverse iteration, forward one could cause us trouble if we
         * delete and erase */
        delete it->second;
    }
    articleSet.clear();
}

Article* ArticleCollection::get(std::string title)
{
    auto it = articleSet.find(title);

    if(articleSet.end() == it) {
        return nullptr;
    }

    return it->second;
}
bool ArticleCollection::add(Article* article)
{
    auto ret = articleSet.insert(std::make_pair(article->getTitle(), article));
    return ret.second;
}

ArticleCollection::iterator ArticleCollection::begin()
{
    return articleSet.begin();
}

ArticleCollection::iterator ArticleCollection::end()
{
    return articleSet.end();
}

ArticleCollection::const_iterator ArticleCollection::begin() const
{
    return articleSet.begin();
}

ArticleCollection::const_iterator ArticleCollection::end() const
{
    return articleSet.end();
}
