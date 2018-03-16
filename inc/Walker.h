//! \file Walker.h

#ifndef WIKIWALKER_WALKER_H
#define WIKIWALKER_WALKER_H

#include "ArticleCollection.h"

namespace WikiWalker
{
  //! Base class for article analyzers
  class Walker
  {
  public:
    /*! get the ArticleCollection with stored articles
     * \returns const reference to collection with stored reference
     * \todo looks code-smelly, is this really neccessary?
     */
    const ArticleCollection& collection() const
    {
      return articleSet_;
    }

    //! virtual base class destructor
    virtual ~Walker() = default;

  protected:
    //! article collection, used as cache, for walked articles
    ArticleCollection articleSet_;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_WALKER_H
