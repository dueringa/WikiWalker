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

  private:
    /*! Convert article to string representation of JSON representation.
     * \param a pointer to article to be converted
     * \return json as string
     * \internal uses the following format:
     * {"title":{"forward_links":[...]}}
     */
    std::string convertToJson(const Article* a);

    /*! Convert article collection to string representation of JSON
     * representation.
     * \param ac reference to article collection to be converted
     * \return json as string
     * \internal uses the following format:
     * {"title":{"forward_links":[...]}, "title2":{"forward_links":[...]}, ...}
     */
    std::string convertToJson(const ArticleCollection& ac);
  };
}  // namespace WikiWalker
#endif  // TOJSONWRITER_H