#ifndef _ARTICLE_H
#define _ARTICLE_H

#include <string>
#include <vector>

class ArticleCollection;

class Article
{
public:
    Article(std::string title);
    ~Article();

    std::string getTitle() const
    {
        return title;
    }

    size_t getNumLinks() const;

    void addLink(Article* article);

private:
    std::string title;
    ArticleCollection* links;
};

#endif //_ARTICLE_H
