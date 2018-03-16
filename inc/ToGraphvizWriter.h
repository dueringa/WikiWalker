//! \file ToGraphvizWriter.h

#ifndef TOGRAPHVIZWRITER_H
#define TOGRAPHVIZWRITER_H

#include "DataOutputBase.h"

namespace WikiWalker
{
  /*! Writing to a file in dot format
   */
  class ToGraphvizWriter : public DataOutputBase
  {
  public:
    /*! output an article with links in dot format to a stream
     * \param ac reference to article collection to be output
     * \param os out stream to putput to
     * YOU are responsible for opening and closing the stream
     */
    void output(const Article* a, std::ostream& os) override;

    /*! output an article collection in dot format to a stream
     * \param ac reference to article collection to be output
     * \param os out stream to putput to
     * YOU are responsible for opening and closing the stream
     */
    void output(const ArticleCollection& ac, std::ostream& os) override;
  };
}  // namespace WikiWalker
#endif  // TOGRAPHVIZWRITER_H
