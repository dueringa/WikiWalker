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
    //! representation of links to other articles
    typedef std::vector<Article*>                 ArticleLinkStorage;
    //! representation of iterator over links
    typedef std::vector<Article*>::iterator ArticleLinkIterator;
    //! representation of const iterator over links
    typedef std::vector<Article*>::const_iterator ArticleLinkConstIterator;

    /*! Create a new article from a title
     * \param title The title of the article
     */
    Article(std::string articleTitle)
        : title(articleTitle), analyzed(false) {};

    //! Get the title of the article
    std::string getTitle() const {
        return title;
    }

    //! get the number of links the article has
    size_t getNumLinks() const;

    /*! Add a link to another article
     * \param[in] article Pointer to the article this article links
     * to
     */
    void addLink(Article* article);

    /*! Set article to be analyzed.
     * State is automatically set by #addLink, but if
     * article has no outgoing links, this must be called,
     * otherwise #getNumLinks will throw an exception
     * \param analyzed whether article has been analyzed
     */
    void setAnalyzed(bool analyzed);

    //! Get state if article was analyzed (for out links)
    bool isAnalyzed() const;

    /*! Get const_iterator to first linked article */
    ArticleLinkConstIterator linkBegin() const;

    /*! Get const_iterator to last linked article */
    ArticleLinkConstIterator linkEnd() const;

private:
    std::string title;
    ArticleLinkStorage links;
    bool analyzed;
};

#endif //_ARTICLE_H
