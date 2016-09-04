#ifndef _WALKEREXCEPTION_H
#define _WALKEREXCEPTION_H

#include <string>

class WalkerException : public std::exception
{
public:
    WalkerException(std::string message)
        : message(message){};
    ~WalkerException() throw(){};
    const char* what() const throw()
    {
        return message.c_str();
    }

private:
    std::string message;
};

#endif // _WALKEREXCEPTION_H