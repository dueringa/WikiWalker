#ifndef _WIKIWALKER_H
#define _WIKIWALKER_H

#include <string>
#include "WalkerException.h"
#include "Walker.h"
#include "CurlWikiGrabber.h"

//! main "app" class
class WikiWalker : public Walker
{
public:
    /*! given an URL (Title?), start collecting links
     * \param url start point for analysis
     */
    void startWalking(std::string url);

private:
    CurlWikiGrabber grabber;

};

#endif // _WIKIWALKER_H
