//! \file ADataOutput.h

#ifndef _ADATAOUTPUT_H
#define _ADATAOUTPUT_H

#include <ostream>
#include "ArticleCollection.h"

/*! Abstract base class for output modules.
 * Child classes are responsible for converting the article structure
 * to other formats.
 */
class DataOutputBase
{
public:
    /*! Output an article to an outstream.
     * \param a pointer to article to be output
     * \param os out stream to putput to.
     * YOU are responsible for opening and closing the stream
     */
    virtual void output(const Article* a, std::ostream& os) = 0;

    /*! Output an article collection to an outstream.
     * \param ac reference to article collection to be output
     * \param os out stream to putput to
     * YOU are responsible for opening and closing the stream
     */
    virtual void output(const ArticleCollection& ac, std::ostream& os) = 0;

    virtual ~DataOutputBase() {}
};

#endif // _ADATAOUTPUT_H