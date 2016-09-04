#ifndef _WALKER_H
#define _WALKER_H

#include <string>
#include "WalkerException.h"

class Walker
{
public:
    Walker(std::string url);

private:
    std::string url;
};

#endif // _WALKER_H