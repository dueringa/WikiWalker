//! \file ArticleCollection.cpp

#include "ArticleCollection.h"

#include <algorithm>
#include <utility>

#include "Article.h"
#include "WalkerException.h"

namespace WikiWalker
{
  namespace CollectionUtils
  {
    static bool articleIsAnalyzed(ArticleCollection::const_reference x)
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

    void merge(ArticleCollection& collection,
               const ArticleCollection& other,
               MergeStrategy strategy)
    {
      switch(strategy) {
        case MergeStrategy::IgnoreDuplicates:
          collection.insert(other.begin(), other.end());
          break;
        case MergeStrategy::AlwaysOverwrite:
          for(const auto& art : other) {
            collection[art.first] = art.second;
          }
          break;
        case MergeStrategy::UseArticleWithMoreLinks:
          for(const auto& art : other) {
            auto articleInThis = collection[art.first];
            auto& otherArt     = art.second;

            if(articleInThis == nullptr ||
               (!articleInThis->analyzed() && otherArt->analyzed())) {
              collection[art.first] = art.second;
              continue;
            }

            if(!otherArt->analyzed()) {
              continue;
            }

            auto linksInThis  = articleInThis->countLinks();
            auto linksInOther = otherArt->countLinks();
            if(linksInOther > linksInThis) {
              collection[art.first] = art.second;
            }
          }
          break;
        default:
          throw WalkerException("Not supported");
      }
    }

    /*! \todo I'm not exactly happy with returning a shared_ptr. Return
     * reference instead? - but how to say "not found" then? */
    std::shared_ptr<Article> get(const ArticleCollection& collection,
                                 const std::string& title)
    {
      auto it = collection.find(title);

      if(collection.end() == it) {
        return nullptr;
      }

      return it->second;
    }
  }  // namespace CollectionUtils

}  // namespace WikiWalker
