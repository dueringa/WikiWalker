//! \file ArticleCollection.h

#ifndef ARTICLE_COLLECTION_H
#define ARTICLE_COLLECTION_H

#include <map>
#include <memory>
#include <string>

// forward-declare, since we only store pointer
class Article;

/*! Collection of available articles.
 *  This should be used as "cache".
 */
class ArticleCollection
{
public:
  //! strategy for merging ArticleCollections
  enum class MergeStrategy {
    //! ignore duplicates, always keep current entry
    IgnoreDuplicates,
    //! always overwrite with articles from other collection
    AlwaysOverwrite,
    // todo: ArticleWithMoreLinks
  };

  //! The way that articles are stored inside
  using storage_type = std::map<std::string, std::shared_ptr<Article>>;

  //! iterator type
  using iterator = storage_type::iterator;

  //! constant iterator type
  using const_iterator = storage_type::const_iterator;

  /*! add article to collection.
   * \param article article to add
   * \return true if insertion took place
   *          false if it failed (e.g. another article with the same title
   * already exists
   */
  bool add(std::shared_ptr<Article> article);

  /*! get number of articles in collection
   * \returns number of articles in collection
   */
  size_t getNumArticles() const
  {
    return articleSet.size();
  }

  /*! merge another ArticleCollection into the current one
   * \param[in] other collection to merge into the current one
   * \param[in] strategy merge stratgy to use
   * \detail articles already present in current collection will be ignored.
   * other collection is left unmodified.
   */
  void merge(const ArticleCollection& other, MergeStrategy strategy);

  /*! get pointer to article.
   * \param title title of the article to request
   * \return pointer to article, or nullptr, if not found
   */
  std::shared_ptr<Article> get(const std::string& title);

  /*! Returns an iterator to the first article in the collection.
   * \returns iterator to the first article
   */
  iterator begin();

  /*! Returns an iterator to the article following the last article in the
   * collection
   * \returns iterator to element after last article
   */
  iterator end();

  /*! Returns a const_iterator to the first article in the collection
   * \returns constant iterator to the first article
   */
  const_iterator begin() const;

  /*! Returns a const_iterator to the article following the last article in the
   * collection
   * \returns constant iterator to element after last article
   */
  const_iterator end() const;

  ArticleCollection() = default;

  /*! deleted copy constructor. Because it stores pointers and I don't want to
   *  do deep copying.
   */
  ArticleCollection(const ArticleCollection&) = delete;

  /*! deleted copy assignment.  Because it stores pointers and I don't want to
   * do deep copying.
   */
  ArticleCollection& operator=(const ArticleCollection&) = delete;

  //! default move constructor
  ArticleCollection(ArticleCollection&&) = default;

  //! default move assignment
  ArticleCollection& operator=(ArticleCollection&&) = default;

private:
  // we need to avoid duplicate article instances.
  // we do this by associating an article title with its instance
  storage_type articleSet;
};

#endif  // ARTICLE_COLLECTION_H
