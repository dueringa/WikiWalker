//! \file ArticleCollection.h

#ifndef ARTICLE_COLLECTION_H
#define ARTICLE_COLLECTION_H

#include <map>
#include <string>
#include <memory>

// forward-declare, since we only store pointer
class Article;

/*! Collection of available articles.
 *  This should be used as "cache".
 */
class ArticleCollection
{
public:
    //! The way that articles are stored inside
    typedef std::map<std::string, std::shared_ptr<Article> > storage_type;

    //! iterator type
    typedef storage_type::iterator iterator;

    //! constant iterator type
    typedef storage_type::const_iterator const_iterator;

    /*! add article to collection.
      * \param article article to add
      * \return true if insertion took place
      *          false if it failed (e.g. another article with the same title already exists
      */
    bool add(std::shared_ptr<Article> article);

    //! get number of articles in collection
    size_t getNumArticles() const
    {
        return articleSet.size();
    }

    /*! get pointer to article.
     * \param title title of the article to request
     * \return pointer to article, or nullptr, if not found
     */
    std::shared_ptr<Article> get(const std::string& title);

    ArticleCollection() {}

    /*! Returns an iterator to the first article in the collection.
     * \returns iterator to the first article
     */
    iterator begin();

    /*! Returns an iterator to the article following the last article in the collection
     * \returns iterator to element after last article
     */
    iterator end();

    /*! Returns a const_iterator to the first article in the collection
     * \returns constant iterator to the first article
     */
    const_iterator begin() const;

    /*! Returns a const_iterator to the article following the last article in the collection
     * \returns constant iterator to element after last article
     */
    const_iterator end() const;

    //! deleted copy constructor. Because it stores raw pointers.
    ArticleCollection(const ArticleCollection&) = delete;
    //! deleted copy assignment. Because it stores raw pointers.
    ArticleCollection& operator=(const ArticleCollection&) = delete;

private:
    // we need to avoid duplicate article instances.
    // we do this by associating an article title with its instance
    storage_type articleSet;
};

#endif // ARTICLE_COLLECTION_H
