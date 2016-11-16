#include "ArticleCollection.h"

ArticleCollection::~ArticleCollection()
{
    /* this is still kinda ugly, since other pointers may exist...
     * also, deleting stack-created articles is going to get ugly
     * (although you shouldn't have pointers to stack variables
     * anyway... */
    for(auto it = articleSet.rbegin(); it != articleSet.rend(); it++) {
        /* reverse iteration, forward one could cause us trouble if we
         * delete and erase */
        delete *it;
    }
    articleSet.clear();
}
