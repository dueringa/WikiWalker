#include "WikiWalker.h"

using namespace std;

void WikiWalker::startWalking(string url)
{
    if(url.find("wikipedia.org/") == string::npos) {
        throw WalkerException("Must be a Wikipedia URL");
    }
    grabber.grabUrl(url);
};
