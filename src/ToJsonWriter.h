//! \file ToJsonWriter.h

#ifndef _TOJSONWRITER_H
#define _TOJSONWRITER_H

#include <string>
#include "DataOutputBase.h"

/*! Output module: write to json.
 * "ugly", non-human-readable JSON, though.
 */
class ToJsonWriter : public DataOutputBase
{
public:
    //! Output JSON data to output stream
    virtual void output(const Article*, std::ostream&) override;
    //! Output JSON data to output stream
    virtual void output(const ArticleCollection&, std::ostream&) override;
private:
    /*! Convert article to string representation of JSON representation.
     * \param a pointer to article to be converted
     * \return json as string
     * \internal uses the following format:
     * {"title":{"forward_links":[...]}}
     */
    std::string convertToJson(const Article* a);

    /*! Convert article collection to string representation of JSON representation.
     * \param ac reference to article collection to be converted
     * \return json as string
     * \internal uses the following format:
     * {"title":{"forward_links":[...]}, "title2":{"forward_links":[...]}, ...}
     */
    std::string convertToJson(const ArticleCollection& ac);
};

#endif // _TOJSONWRITER_H
