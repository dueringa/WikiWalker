#ifndef _TOJSONWRITER_H
#define _TOJSONWRITER_H

#include <string>
#include "Article.h"
#include "ArticleCollection.h"
#include "ADataOutput.h"

class ToJsonWriter : public ADataUsage
{
public:
    std::string convertToJson(const Article*);
    std::string convertToJson(const ArticleCollection&);

    virtual void output(const Article*, std::ostream&);
    virtual void output(const ArticleCollection&, std::ostream&);
};

#endif // _TOJSONWRITER_H
