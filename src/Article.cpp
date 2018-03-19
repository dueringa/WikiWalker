//! \file Article.cpp

#include "Article.h"

#include <algorithm>

#include "WalkerException.h"

namespace WikiWalker
{
  size_t Article::countLinks() const
  {
    if(!analyzed_ && links_.empty()) {
      throw WalkerException("Article not analyzed yet!");
    }

    return links_.size();
  }

  Article::ArticleLinkConstIterator Article::linkBegin() const
  {
    return links_.cbegin();
  }

  Article::ArticleLinkConstIterator Article::linkEnd() const
  {
    return links_.cend();
  }

  bool Article::addLink(const link article)
  {
    auto newTitle = article.lock()->title();

    // check for duplicates using title
    //! \todo Or rather compare pointers again?
    bool isNewLink =
        std::none_of(links_.cbegin(), links_.cend(), [&newTitle](const link x) {
          auto p = x.lock();
          return p == nullptr ? false : p->title() == newTitle;
        });

    if(isNewLink) {
      links_.push_back(article);
      analyzed_ = true;
    }

    return isNewLink;
  }

  void Article::analyzed(bool analyzed)
  {
    analyzed_ = analyzed;
  }

  bool Article::analyzed() const
  {
    return analyzed_;
  }

  void Article::marked(bool marked)
  {
    marked_ = marked;
  }

  bool Article::marked() const
  {
    return marked_;
  }
}  // namespace WikiWalker