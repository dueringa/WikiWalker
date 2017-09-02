//! \file WalkerException.h

#ifndef WALKEREXCEPTION_H
#define WALKEREXCEPTION_H

#include <stdexcept>
#include <string>

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
  WalkerException(std::string exmessage) : runtime_error(exmessage)
  {
  }
};

#endif  // WALKEREXCEPTION_H
