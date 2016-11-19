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
     * \param message The exception message.
     */
    WalkerException(std::string message)
        : message(message) {};

    ~WalkerException() throw() {};

    //! get exception message
    const char* what() const throw()
    {
        return message.c_str();
    }

private:
    std::string message;
};

#endif // _WALKEREXCEPTION_H
