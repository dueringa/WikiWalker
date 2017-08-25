//! \file WalkerException.h

#ifndef WALKEREXCEPTION_H
#define WALKEREXCEPTION_H

#include <string>

//! (base) class for exceptions in WikiWalker
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

    virtual ~WalkerException() noexcept {}

    //! get exception message
    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

#endif // WALKEREXCEPTION_H
