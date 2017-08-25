//! \file ToGraphvizWriter.h

#ifndef TOGRAPHVIZWRITER_H
#define TOGRAPHVIZWRITER_H

#include "DataOutputBase.h"

/*! Writing to a file in dot format
 */
class ToGraphvizWriter : public DataOutputBase
{
public:
    virtual void output(const Article* a, std::ostream& os) override;
    virtual void output(const ArticleCollection& ac, std::ostream& os) override;
private:
    void writeHeader(std::ostream& os);
    void writeFooter(std::ostream& os);
    void writeArticle(const Article* a, std::ostream& os);
};

#endif // TOGRAPHVIZWRITER_H
