#include "Article.h"

size_t Article::getNumLinks() const
{
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
