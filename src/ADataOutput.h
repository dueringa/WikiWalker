#ifndef _ADATAOUTPUT_H
#define _ADATAOUTPUT_H

#include <ostream>
#include "ArticleCollection.h"

/*! Abstract base class for output modules.
 * Child classes are responsible for converting the article structure
 * to other formats.
 */
class ADataOutput
{
public:
    /*! Output an article to an outstream.
     * \param a pointer to article to be output
     * \param os out stream to putput to
     */
    virtual void output(const Article* a, std::ostream& os) = 0;

    /*! Output an article collection to an outstream.
     * \param ac reference to article collection to be output
     * \param os out stream to putput to
     */
    virtual void output(const ArticleCollection& ac, std::ostream& os) = 0;

    virtual ~ADataOutput() {}
};

#endif // _ADATAOUTPUT_H
