//! \file ToJsonWriter.h

#ifndef TOJSONWRITER_H
#define TOJSONWRITER_H

#include <string>

#include "DataOutputBase.h"

namespace WikiWalker
{
  /*! Output module: write to json.
   * "ugly", non-human-readable JSON, though.
   */
  class ToJsonWriter : public DataOutputBase
  {
  public:
    /*! Output JSON data to output stream
     * \param a pointer to article to be output
     * \param os out stream to putput to.
     * YOU are responsible for opening and closing the stream
     */
    void output(const Article* article, std::ostream& outstream) override;

    /*! Output JSON data to output stream
     * \param ac reference to article collection to be output
     * \param os out stream to putput to
     * YOU are responsible for opening and closing the stream
     */
    void output(const ArticleCollection& collection,
                std::ostream& outstream) override;
  };
}  // namespace WikiWalker
#endif  // TOJSONWRITER_H
