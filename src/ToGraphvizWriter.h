#ifndef _TOGRAPHVIZWRITER_H
#define _TOGRAPHVIZWRITER_H

#include "ADataOutput.h"

/*! Writing to a file in dot format
 */
class ToGraphvizWriter : public ADataOutput
{
public:
    virtual void output(const Article* a, std::ostream& os);
    virtual void output(const ArticleCollection& ac, std::ostream& os);
private:
    void writeHeader(std::ostream& os);
    void writeFooter(std::ostream& os);
    void writeArticle(const Article *a, std::ostream& os);
};

#endif // _TOGRAPHVIZWRITER_H
