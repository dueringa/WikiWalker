//! \file StringUtils.h

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>

namespace WikiWalker
{
  //! A collection of string utilities
  namespace StringUtils
  {
    /*!
     * Checks whether a string begins with another string.
     * \param stringToTest the string that should be tested
     * \param beginning the search string
     * \returns true if first string begins with the second one, otherwise
     * false. Also returns false if either string is empty.
     */
    bool startsWith(const std::string& stringToTest,
                    const std::string& beginning);
  }  //  namespace StringUtils
}  //  namespace WikiWalker

#endif /* STRINGUTILS_H */
