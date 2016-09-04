#ifndef _WIKIWALKER_H
#define _WIKIWALKER_H

#include <string>
#include "WalkerException.h"
#include "Walker.h"

class WikiWalker : public Walker
{
public:
    WikiWalker(std::string url);

private:
    std::string url;
};

#endif // _WIKIWALKER_H