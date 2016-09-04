#include "Walker.h"

using namespace std;

Walker::Walker(string url)
    : url(url)
{
    if(url.find("wikipedia.org/") == string::npos) {
        throw WalkerException("Must be a Wikipedia URL");
    }
};