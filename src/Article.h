#ifndef _ARTICLE_H
#define _ARTICLE_H

#include <string>
#include <vector>

/*!
 * represents a Wikipedia (Mediawiki) and its links
 */
class Article
{
public:
    typedef std::vector<Article*>                 ArticleLinkStorage;
    typedef std::vector<Article*>::const_iterator ArticleLinkIterator;

    /*! Create a new article from a title
     * \param title The title of the article
     */
    Article(std::string title)
        : title(title) {};

    //! Get the title of the article
    std::string getTitle() const
    {
        return title;
    }

    //! get the number of links the article has
    size_t getNumLinks() const;

    /*! Add a link to another article
     * \param[in] article Pointer to the article this article links
     * to
     */
    void addLink(Article* article)
    {
        links.push_back(article);
    }

    /*! Get const_iterator to first linked article */
    ArticleLinkIterator linkBegin() const
    {
        return links.cbegin();
    }

    /*! Get const_iterator to last linked article */
    ArticleLinkIterator linkEnd() const
    {
        return links.cend();
    }

private:
    std::string title;
    ArticleLinkStorage links;
};

#endif //_ARTICLE_H
