//! \file ArticleCollection.cpp

#include "ArticleCollection.h"

#include <algorithm>
#include <utility>

#include "Article.h"
#include "WalkerException.h"

namespace WikiWalker
{
  /*! \todo I'm not exactly happy with returning a shared_ptr. Return
   * reference instead? - but how to say "not found" then? */
  std::shared_ptr<Article> ArticleCollection::get(const std::string& title)
  {
    auto it = articleSet_.find(title);

    if(articleSet_.end() == it) {
      return nullptr;
    }

    return it->second;
  }

  ArticleCollection::mapped_type& ArticleCollection::operator[](
      const ArticleCollection::key_type& key)
  {
    return articleSet_[key];
  }

  std::pair<ArticleCollection::iterator, bool> ArticleCollection::insert(
      value_type value)
  {
    return articleSet_.insert(value);
  }

  void ArticleCollection::insert(ArticleCollection::const_iterator first,
                                 ArticleCollection::const_iterator last)
  {
    return articleSet_.insert(first, last);
  }

  void ArticleCollection::merge(const ArticleCollection& other,
                                CollectionUtils::MergeStrategy strategy)
  {
    switch(strategy) {
      case CollectionUtils::MergeStrategy::IgnoreDuplicates:
        articleSet_.insert(other.begin(), other.end());
        break;
      case CollectionUtils::MergeStrategy::AlwaysOverwrite:
        for(const auto& art : other) {
          articleSet_[art.first] = art.second;
        }
        break;
      case CollectionUtils::MergeStrategy::UseArticleWithMoreLinks:
        for(const auto& art : other) {
          auto articleInThis = articleSet_[art.first];
          auto& otherArt     = art.second;

          if(articleInThis == nullptr ||
             (!articleInThis->analyzed() && otherArt->analyzed())) {
            articleSet_[art.first] = art.second;
            continue;
          }

          if(!otherArt->analyzed()) {
            continue;
          }

          auto linksInThis  = articleInThis->countLinks();
          auto linksInOther = otherArt->countLinks();
          if(linksInOther > linksInThis) {
            articleSet_[art.first] = art.second;
          }
        }
        break;
      default:
        throw WalkerException("Not supported");
    }
  }

  ArticleCollection::iterator ArticleCollection::begin()
  {
    return articleSet_.begin();
  }

  ArticleCollection::iterator ArticleCollection::end()
  {
    return articleSet_.end();
  }

  ArticleCollection::const_iterator ArticleCollection::begin() const
  {
    return articleSet_.begin();
  }

  ArticleCollection::const_iterator ArticleCollection::end() const
  {
    return articleSet_.end();
  }

  namespace CollectionUtils
  {
    static bool articleIsAnalyzed(
        ArticleCollection::storage_type::const_reference x)
    {
      auto art = x.second;
      if(art == nullptr) {
        return false;
      }
      return art->analyzed();
    }

    size_t countAnalyzedArticles(const ArticleCollection& collection)
    {
      return std::count_if(
          collection.begin(), collection.end(), articleIsAnalyzed);
    }

    bool add(ArticleCollection& collection, std::shared_ptr<Article> article)
    {
      auto ret = collection.insert(std::make_pair(article->title(), article));
      return ret.second;
    }
  }  // namespace CollectionUtils

}  // namespace WikiWalker