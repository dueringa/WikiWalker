#include "WikiWalker.h"

using namespace std;

void WikiWalker::startWalking(string url)
{
    std::string findUrl = "en.wikipedia.org/wiki/";
    size_t pos = url.find(findUrl);
    if(pos == string::npos) {
        throw WalkerException("Must be an English Wikipedia URL");
    }

    // extract Wikipedia title
    std::string title = url.substr(pos + findUrl.length());

    // little bobby tables?
    auto content = grabber.grabUrl("https://en.wikipedia.org/w/api.php?action=query&format=json&prop=links&pllimit=100&titles=" + title);
};
