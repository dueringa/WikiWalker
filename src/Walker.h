//! \file Walker.h

#ifndef _WALKER_H
#define _WALKER_H

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

#endif // _WALKER_H
