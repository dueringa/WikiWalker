//! \file Article.h

#ifndef WIKIWALKER_ARTICLE_H
#define WIKIWALKER_ARTICLE_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace WikiWalker
{
  /*!
   * represents a Wikipedia (Mediawiki) article and its links
   */
  class Article
  {
  public:
    //! representation of links to other articles
    using storage = std::vector<std::weak_ptr<const Article>>;
    //! representation of iterator over links
    using ArticleLinkIterator = storage::iterator;
    //! representation of const iterator over links
    using ArticleLinkConstIterator = storage::const_iterator;

    /*! Create a new article from a title
     * \param articleTitle The title of the article
     */
    explicit Article(std::string articleTitle)
        : title_(std::move(articleTitle)), analyzed_(false), marked_(false)
    {
    }

    /*! Get the title of the article
     * \return title of the article
     */
    std::string title() const
    {
      return title_;
    }

    /*! get the number of links the article has.
     * This throws an exception if state has not been set to anaylzed.
     * \return number of links the article has.
     * This also includes links to articles that are nullptr / already deleted
     * by the smart pointers.
     *
     * \see analyzed(bool)
     * \see addLink
     */
    size_t countLinks() const;

    /*! Add a link to another article.
     * \param[in] article Pointer to the article this article links
     * to
     * \returns Whether adding woth successful. Returns false if
     * instance / pointer is already included.
     *
     * Automatically sets state to analyzed
     * \see analyzed() const
     * \see analyzed(bool)
     */
    bool addLink(const std::weak_ptr<const Article> article);

    /*! Set article to be analyzed.
     * State is automatically set by #addLink, but if
     * article has no outgoing links, this must be called,
     * otherwise #numLinks will throw an exception
     * \param analyzed whether article has been analyzed
     * \see addLink
     */
    void analyzed(bool analyzed);

    /*! Get state if article was analyzed (for out links).
     * \returns whether article was analyzed for outgoing links
     */
    bool analyzed() const;

    /*! Set article to be marked.
     * Some kind of "marking" for output usage. May be start point,
     * may be end point, may be point of special interest.
     * \param marked whether article is marked
     */
    void marked(bool marked);

    /*! Get state whether article was marked.
     * \returns Whether article is marked.
     */
    bool marked() const;

    /*! Get const_iterator to first linked article
     * \returns const iterator to beginning of linked articles
     */
    ArticleLinkConstIterator linkBegin() const;

    /*! Get const_iterator to last linked article
     * \returns const iterator to end of linked articles (behind last one)
     */
    ArticleLinkConstIterator linkEnd() const;

  private:
    std::string title_;
    storage links_;
    bool analyzed_;
    bool marked_;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_ARTICLE_H
