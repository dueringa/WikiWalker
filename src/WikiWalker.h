#ifndef _WIKIWALKER_H
#define _WIKIWALKER_H

#include <string>
#include "WalkerException.h"

class WikiWalker
{
public:
    WikiWalker(std::string url);

private:
    std::string url;
};

#endif // _WIKIWALKER_H