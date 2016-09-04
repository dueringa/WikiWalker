#include "WikiWalker.h"

using namespace std;

WikiWalker::WikiWalker(string url)
    : url(url)
{
    if(url.find("wikipedia.org/") == string::npos) {
        throw WalkerException("Must be a Wikipedia URL");
    }
};