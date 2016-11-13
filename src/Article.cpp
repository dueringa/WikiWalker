#include "Article.h"
#include "ArticleCollection.h"

Article::Article(std::string title)
    : title(title)
{
    links = new ArticleCollection();
};

Article::~Article()
{
    delete links;
}

size_t Article::getNumLinks() const
{
    return links->getNumArticles();
}

void Article::addLink(Article* article)
{
    links->add(article);
}
