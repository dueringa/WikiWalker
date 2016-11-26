//! \file WikiWalker.h

#ifndef _WIKIWALKER_H
#define _WIKIWALKER_H

#include <string>
#include "Walker.h"
#include "CurlWikiGrabber.h"

//! main "app" class
class WikiWalker : public Walker
{
public:
    /*! given an URL, start collecting links
     * \param url start point for analysis
     */
    void startWalking(std::string url);

    void readCache(std::string cache);
    void writeCache(std::string cache);

private:
    CurlWikiGrabber grabber;

};

#endif // _WIKIWALKER_H
