//! \file Walker.h

#ifndef WALKER_H
#define WALKER_H

#include "ArticleCollection.h"

//! Base class for article analyzers
class Walker
{
public:
    const ArticleCollection& getCollection() const
    {
        return articleSet;
    }

protected:
    //! article collection, used as cache, for walked articles
    ArticleCollection articleSet;
};

#endif // WALKER_H
