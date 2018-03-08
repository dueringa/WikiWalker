//! \file StringUtils.h

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>

namespace WikiWalker
{
  //! A collection of string utilities
  class StringUtils
  {
  public:
    /*!
     * Checks whether a string begins with another string.
     * \param stringToTest the string that should be tested
     * \param beginning the search string
     * \returns true if first string begins with the second one, otherwise
     * false. Also returns false if either string is empty.
     */
    static bool startsWith(std::string stringToTest, std::string beginning);
  };
}

#endif /* STRINGUTILS_H */
