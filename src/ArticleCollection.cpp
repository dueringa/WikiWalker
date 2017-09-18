//! \file ArticleCollection.cpp

#include "ArticleCollection.h"

#include <utility>

#include "Article.h"
#include "WalkerException.h"

using namespace WikiWalker;

/*! \todo I'm not exactly happy with returning a shared_ptr. Return
 * reference instead? - but how to say "not found" then? */
std::shared_ptr<Article> ArticleCollection::get(const std::string& title)
{
  auto it = articleSet.find(title);

  if(articleSet.end() == it) {
    return nullptr;
  }

  return it->second;
}

bool ArticleCollection::add(std::shared_ptr<Article> article)
{
  auto ret = articleSet.insert(std::make_pair(article->getTitle(), article));
  return ret.second;
}

void ArticleCollection::merge(const ArticleCollection& other,
                              ArticleCollection::MergeStrategy strategy)
{
  switch(strategy) {
    case ArticleCollection::MergeStrategy::IgnoreDuplicates:
      articleSet.insert(other.begin(), other.end());
      break;
    case ArticleCollection::MergeStrategy::AlwaysOverwrite:
      for(const auto& art : other) {
        articleSet[art.first] = art.second;
      }
      break;
    default:
      throw WalkerException("Not supported");
  }
}

ArticleCollection::iterator ArticleCollection::begin()
{
  return articleSet.begin();
}

ArticleCollection::iterator ArticleCollection::end()
{
  return articleSet.end();
}

ArticleCollection::const_iterator ArticleCollection::begin() const
{
  return articleSet.begin();
}

ArticleCollection::const_iterator ArticleCollection::end() const
{
  return articleSet.end();
}
