//! \file

#ifndef WIKIWALKER_JSONSERIALIZER_H
#define WIKIWALKER_JSONSERIALIZER_H

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

    /*! Deserialize JSON data to an ArticleCollection
     * \param instream the stream containing the JSON data
     * \returns The desirialized article collection
     */
    ArticleCollection deserialize(std::istream& instream);

    /*! Deserialize JSON data to an ArticleCollection
     * \param[out] collection The desirialized article collection
     * \param instream the stream containing the JSON data
     */
    void deserialize(ArticleCollection& collection, std::istream& instream);
  };
}
#endif  // WIKIWALKER_JSONSERIALIZER_H
