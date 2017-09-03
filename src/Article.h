//! \file Article.h

#ifndef ARTICLE_H
#define ARTICLE_H

#include <memory>
#include <string>
#include <vector>

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
      : title(articleTitle), analyzed(false), marked(false)
  {
  }

  /*! Get the title of the article
   * \return title of the article
   */
  std::string getTitle() const
  {
    return title;
  }

  /*! get the number of links the article has.
   * This throws an exception if state has not been set to anaylzed.
   * \return number of links the article has.
   * \see setAnalyzed
   * \see addLink
   */
  size_t getNumLinks() const;

  /*! Add a link to another article.
   * \param[in] article Pointer to the article this article links
   * to
   * \returns Whether adding woth successful. Returns false if
   * instance / pointer is already included.
   *
   * Automatically sets state to analyzed
   * \see setAnalyzed
   * \see isAnalyzed
   */
  bool addLink(std::weak_ptr<const Article> article);

  /*! Set article to be analyzed.
   * State is automatically set by #addLink, but if
   * article has no outgoing links, this must be called,
   * otherwise #getNumLinks will throw an exception
   * \param analyzed whether article has been analyzed
   * \see addLink
   */
  void setAnalyzed(bool analyzed);

  //! Get state if article was analyzed (for out links)
  bool isAnalyzed() const;

  /*! Set article to be marked.
   * Some kind of "marking" for output usage. May be start point,
   * may be end point, may be point of special interest.
   * \param marked whether article os marked
   */
  void setMarked(bool marked);

  /*! Get state whether article was marked.
   * \returns Whether article is marked.
   */
  bool isMarked() const;

  /*! Get const_iterator to first linked article */
  ArticleLinkConstIterator linkBegin() const;

  /*! Get const_iterator to last linked article */
  ArticleLinkConstIterator linkEnd() const;

private:
  std::string title;
  storage links;
  bool analyzed;
  bool marked;
};

#endif  // ARTICLE_H
