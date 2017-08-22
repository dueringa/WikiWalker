//! \file Article.cpp

#include "Article.h"
#include "WalkerException.h"
#include <algorithm>

size_t Article::getNumLinks() const
{
    if(!analyzed && links.empty()) {
        throw WalkerException("Article not analyzed yet!");
    }

    return links.size();
}

Article::ArticleLinkConstIterator Article::linkBegin() const
{
    return links.cbegin();
}

Article::ArticleLinkConstIterator Article::linkEnd() const
{
    return links.cend();
}

bool Article::addLink(Article::stored_type article)
{
    //! TODO: figure out how to find duplicates with weak_ptr
    /*
    auto pos = find(links.begin(), links.end(), article);

    if(pos != links.end()) {
        return false;
    }
    */

    links.push_back(article);
    analyzed = true;

    return true;
}

void Article::setAnalyzed(bool analyzedState)
{
    analyzed = analyzedState;
}

bool Article::isAnalyzed() const
{
    return analyzed;
}

void Article::setMarked(bool _marked)
{
    marked = _marked;
}

bool Article::isMarked() const
{
    return marked;
}
