//! \file WalkerException.h

#ifndef WIKIWALKER_WALKEREXCEPTION_H
#define WIKIWALKER_WALKEREXCEPTION_H

#include <stdexcept>
#include <string>

namespace WikiWalker
{
  //! (base) class for exceptions in WikiWalker
  class WalkerException : public std::runtime_error
  {
  public:
    /*! Create a Walker exception with a message.
     *
     * Message might be shown on exception occurring, depending on
     * the compiler.
     *
     * \param exmessage The exception message.
     */
    explicit WalkerException(const std::string& exmessage)
        : runtime_error(exmessage)
    {
    }
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_WALKEREXCEPTION_H
