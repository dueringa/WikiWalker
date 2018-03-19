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
     * YOU are responsible for opening and closing the stream.
     * Articles in the collection which happen to "link" against nullptrs
     * because their weak_ptr already expired will be simply *omitted* from the
     * serialization
     */
    void serialize(const ArticleCollection& collection,
                   std::ostream& outstream);

    /*! Deserialize JSON data to an ArticleCollection
     * \param instream the stream containing the JSON data
     * \returns The desirialized article collection.
     * \sa deserialize(ArticleCollection&, std::istream&)
     */
    ArticleCollection deserialize(std::istream& instream);

    /*! Deserialize JSON data to an ArticleCollection
     * \param[out] collection The desirialized article collection
     * \param instream the stream containing the JSON data
     * \details The deserialized object / collection might not exactly match the
     * serialized one! If a link of an article which is inside the article
     * collection doesn't exist in the collection on serialization, it will be
     * added to the collection on deserialization.
     *
     * Example: "A" is in the collection and links to "B", but "B" is not in the
     * collection itself, so `["A" -> [ "B" ]]`. On deserialization, however,
     * "B" is added to the collection, so the collection looks like `["A" -> [
     * "B" ], "B" -> null]`. This is, the "new" article B is added and marked as
     * non-analyzed.
     *
     * This is because of the implementation of the links as #std::weak_ptr - on
     * serialization, they could still be in scope, but on deserialization, I
     * need some #std::shared_ptr so the object doesn't disappear right away.
     * This is why I deserialize them as new object in the collection.
     */
    void deserialize(ArticleCollection& collection, std::istream& instream);
  };
}
#endif  // WIKIWALKER_JSONSERIALIZER_H
