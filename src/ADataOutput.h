#ifndef _ADATAOUTPUT_H
#define _ADATAOUTPUT_H

#include <ostream>
#include "ArticleCollection.h"

class ADataOutput
{
public:
    virtual void output(const Article*, std::ostream&) = 0;
    virtual void output(const ArticleCollection&, std::ostream&) = 0;

    virtual ~ADataOutput() {}
};

#endif // _ADATAOUTPUT_H
