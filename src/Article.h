#ifndef _ARTICLE_H
#define _ARTICLE_H

#include <string>
#include <vector>

class Article
{
public:
    Article(std::string title)
        : title(title){};

    std::string getTitle() const
    {
        return title;
    }

    size_t getNumLinks() const;

private:
    std::string title;
    std::vector<Article*> links;
};

#endif //_ARTICLE_H