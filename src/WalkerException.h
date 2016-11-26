//! \file WalkerException.h

#ifndef _WALKEREXCEPTION_H
#define _WALKEREXCEPTION_H

#include <string>

//! (base) class for exceptions in uvok/WikiWalker
class WalkerException : public std::exception
{
public:
    /*! Create a Walker exception with a message.
     *
     * Message might be shown on exception occurring, depending on
     * the compiler.
     *
     * \param exmessage The exception message.
     */
    WalkerException(std::string exmessage)
        : message(exmessage) {}

    virtual ~WalkerException() throw() {}

    //! get exception message
    const char* what() const noexcept
    {
        return message.c_str();
    }

private:
    std::string message;
};

#endif // _WALKEREXCEPTION_H
