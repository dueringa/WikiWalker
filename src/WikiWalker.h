#ifndef _WIKIWALKER_H
#define _WIKIWALKER_H

#include <string>
#include "WalkerException.h"
#include "Walker.h"

class WikiWalker : public Walker
{
public:
    void startWalking(std::string url);

private:

};

#endif // _WIKIWALKER_H