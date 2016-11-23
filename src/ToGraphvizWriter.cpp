#include "ToGraphvizWriter.h"

#include <string>

void ToGraphvizWriter::writeHeader(std::ostream& os)
{
    os << "digraph G {";
    os << std::endl;
}

void ToGraphvizWriter::writeFooter(std::ostream& os)
{
    os << "}";
    os << std::endl;
}

void ToGraphvizWriter::writeArticle(const Article *a, std::ostream& os)
{
    for(auto al = a->linkBegin(); al != a->linkEnd(); al++) {
        os << a->getTitle() << " -> " << (*al)->getTitle() << std::endl;
    }
}

void ToGraphvizWriter::output(const Article* a, std::ostream& os)
{
    writeHeader(os);

    writeArticle(a, os);

    writeFooter(os);
}

void ToGraphvizWriter::output(const ArticleCollection& ac, std::ostream& os)
{
    writeHeader(os);

    for(auto a : ac) {
        writeArticle(a.second, os);
    }

    writeFooter(os);
}
