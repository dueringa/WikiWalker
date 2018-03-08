//! \file Walker.h

#ifndef WALKER_H
#define WALKER_H

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
    const ArticleCollection& getCollection() const
    {
      return articleSet;
    }

    //! virtual base class destructor
    virtual ~Walker() = default;

  protected:
    //! article collection, used as cache, for walked articles
    ArticleCollection articleSet;
  };
}
#endif  // WALKER_H
