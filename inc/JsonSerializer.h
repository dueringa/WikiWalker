//! \file

#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <iostream>

#include "ArticleCollection.h"

namespace WikiWalker
{
  /*! Serialize AricleCollection from and to a custom JSON format
   */
  class JsonSerializer
  {
  public:
    /*! Serialize ArticleCollection to JSON in an output stream
     * \param a pointer to article to be output
     * \param os out stream to putput to.
     * YOU are responsible for opening and closing the stream
     */
    void serialize(const ArticleCollection& collection,
                   std::ostream& outstream);
  };
}
#endif  // JSONSERIALIZER_H
