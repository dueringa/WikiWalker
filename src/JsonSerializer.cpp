//! \file

#include "JsonSerializer.h"
#include "ToJsonWriter.h"

namespace WikiWalker
{
  void JsonSerializer::serialize(const ArticleCollection& collection,
                                 std::ostream& outstream)
  {
    ToJsonWriter tjw;
    tjw.output(collection, outstream);
  }
}
