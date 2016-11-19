#include "Article.h"
#include "WalkerException.h"

size_t Article::getNumLinks() const
{
    if(!analyzed && !links.size()) {
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

void Article::addLink(Article* article)
{
    links.push_back(article);
    analyzed = true;
}

void Article::setAnalyzed(bool analyzedState) {
    analyzed = analyzedState;
}

bool Article::isAnalyzed() const {
    return analyzed;
}
