#include <UnitTest++.h>

#include "../src/ArticleCollection.h"

SUITE(CollectionTests)
{
    TEST(MakeSureNoDuplicateArticlesExist)
    {
        ArticleCollection w;
        w.add(new Article("King"));
        w.add(new Article("Queen"));
        CHECK_EQUAL(2, w.getNumArticles());
        w.add(new Article("Prince"));
        CHECK_EQUAL(3, w.getNumArticles());
        w.add(new Article("Queen"));
        CHECK_EQUAL(3, w.getNumArticles());
    }
}
