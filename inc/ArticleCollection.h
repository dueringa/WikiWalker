//! \file ArticleCollection.h

#ifndef WIKIWALKER_ARTICLE_COLLECTION_H
#define WIKIWALKER_ARTICLE_COLLECTION_H

#include <map>
#include <memory>
#include <string>

// forward-declare, since we only store pointer
namespace WikiWalker
{
  class Article;
}  //  namespace WikiWalker

namespace WikiWalker
{
  /*! Collection of available articles.
   *  This should be used as "cache".
   */
  using ArticleCollection = std::map<std::string, std::shared_ptr<Article>>;

  namespace CollectionUtils
  {
    //! strategy for merging ArticleCollections
    enum class MergeStrategy {
      //! ignore duplicates, always keep current entry
      IgnoreDuplicates,
      //! always overwrite with articles from other collection
      AlwaysOverwrite,
      //! use the article with more links
      UseArticleWithMoreLinks
    };

    /*! get number of anlyzed articles in collection
     * \returns number of analyzed articles in collection
     */
    size_t countAnalyzedArticles(const ArticleCollection& collection);

    /*! add article to collection.
     * \param collection collection to add to
     * \param article article to add
     * \return true if insertion took place
     *          false if it failed (e.g. another article with the same title
     *          already exists
     */
    bool add(ArticleCollection& collection, std::shared_ptr<Article> article);

    /*! merge another ArticleCollection into the first one
     * \param[in] collection collection to merge into
     * \param[in] other collection to merge
     * \param[in] strategy merge stratgy to use
     * \details other collection is left unmodified.
     */
    void merge(ArticleCollection& collection,
               const ArticleCollection& other,
               CollectionUtils::MergeStrategy strategy);

    /*! get pointer to article.
     * \param collection collection to search
     * \param title title of the article to request
     * \return pointer to article, or nullptr, if not found
     */
    std::shared_ptr<Article> get(const ArticleCollection& collection,
                                 const std::string& title);
  }  // namespace CollectionUtils
}  // namespace WikiWalker
#endif  // WIKIWALKER_ARTICLE_COLLECTION_H
