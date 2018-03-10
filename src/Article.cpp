//! \file Article.cpp

#include "Article.h"

#include <algorithm>

#include "WalkerException.h"

namespace WikiWalker
{
  size_t Article::getNumLinks() const
  {
    if(!analyzed && links.empty()) {
      throw WalkerException("Article not analyzed yet!");
    }

    return links.size();
  }

  Article::ArticleLinkConstIterator Article::linkBegin() const
  {
    return links.cbegin();
  }

  Article::ArticleLinkConstIterator Article::linkEnd() const
  {
    return links.cend();
  }

  bool Article::addLink(std::weak_ptr<const Article> article)
  {
    auto newTitle = article.lock()->getTitle();

    // check for duplicates using title
    //! \todo Or rather compare pointers again?
    bool isNewLink =
        std::none_of(links.cbegin(),
                     links.cend(),
                     [&newTitle](const std::weak_ptr<const Article> x) {
                       return x.lock()->getTitle() == newTitle;
                     });

    if(isNewLink) {
      links.push_back(article);
      analyzed = true;
    }

    return isNewLink;
  }

  void Article::setAnalyzed(bool analyzedState)
  {
    analyzed = analyzedState;
  }

  bool Article::isAnalyzed() const
  {
    return analyzed;
  }

  void Article::setMarked(bool _marked)
  {
    marked = _marked;
  }

  bool Article::isMarked() const
  {
    return marked;
  }
}