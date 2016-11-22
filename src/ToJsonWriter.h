#ifndef _TOJSONWRITER_H
#define _TOJSONWRITER_H

#include <string>
#include "Article.h"
#include "ArticleCollection.h"

class ToJsonWriter
{
public:
    std::string convertToJson(const Article*);
    std::string convertToJson(const ArticleCollection&);
};

#endif // _TOJSONWRITER_H

