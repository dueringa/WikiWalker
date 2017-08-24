//! \file ToGraphvizWriter.cpp

#include "ToGraphvizWriter.h"

#include <string>

#include "Article.h"

// uh-oh...
#include <regex>

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

/*! \bug When writing an #Article only, attributes are only set on the article
 * itself. Attributes won't be written on linked articles.
 * However, when writing an #ArticleCollection, all articles are included, so
 * all attibutes will be written.
 */
void ToGraphvizWriter::writeArticle(const Article* a, std::ostream& os)
{
    std::string atitle = a->getTitle();

    // search for quotes
    std::regex re("\"");
    // replace by escaped quote
    atitle = std::regex_replace(atitle, re, "\\\"");

    // marked articles are printed as box
    if(a->isMarked()) {
        os << "\"" << atitle << "\" [shape=box];" << std::endl;
    }

    if(!a->isAnalyzed()) {
        os << "\"" << atitle << "\" [fillcolor=gray,style=filled];" << std::endl;
    }

    // unanalyzed articles are printed greyed out
    for(auto al = a->linkBegin(); al != a->linkEnd(); al++) {
        auto lck_article = al->lock();
        if(lck_article != nullptr) {
            std::string alinkedtitle = lck_article->getTitle();
            os << "\"" << atitle << "\" -> \""
                << std::regex_replace(alinkedtitle, re, "\\\"")
                << "\";" << std::endl;
        }
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
        writeArticle(a.second.get(), os);
    }

    writeFooter(os);
}
