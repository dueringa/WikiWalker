//! \file

#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <iostream>

#include "ArticleCollection.h"

namespace WikiWalker
{
  class JsonSerializer
  {
  public:
    void serialize(const ArticleCollection& collection,
                   std::ostream& outstream);
  };
}
#endif  // JSONSERIALIZER_H
