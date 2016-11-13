#ifndef _WIKIWALKER_H
#define _WIKIWALKER_H

#include <string>
#include "WalkerException.h"
#include "Walker.h"
#include "CurlWikiGrabber.h"

class WikiWalker : public Walker
{
public:
    void startWalking(std::string url);

private:
    CurlWikiGrabber grabber;

};

#endif // _WIKIWALKER_H
