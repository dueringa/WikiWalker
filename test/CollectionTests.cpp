#include <UnitTest++.h>

#include "../src/ArticleCollection.h"
#include "../src/Article.h"

SUITE(CollectionTests)
{
    TEST(MakeSureNoDuplicateArticlesExist)
    {
        ArticleCollection w;
        CHECK(w.add(new Article("King")));
        CHECK(w.add(new Article("Queen")));
        CHECK_EQUAL(2, w.getNumArticles());
        CHECK(w.add(new Article("Prince")));
        CHECK_EQUAL(3, w.getNumArticles());

        // must fail
        CHECK(!w.add(new Article("Queen")));
        CHECK_EQUAL(3, w.getNumArticles());
    }

    TEST(CollectionIsCaseInsensitive)
    {
        ArticleCollection w;
        w.add(new Article("King"));
        w.add(new Article("Queen"));
        CHECK_EQUAL(2, w.getNumArticles());
        w.add(new Article("Prince"));
        CHECK_EQUAL(3, w.getNumArticles());
        w.add(new Article("queen"));
        CHECK_EQUAL(4, w.getNumArticles());
    }

    TEST(GetArticle_Existing_MustNotBeNull)
    {
        ArticleCollection w;
        w.add(new Article("King"));
        CHECK(w.get("King") != nullptr);
    }

    TEST(GetArticle_NonExisting_MustBeNull)
    {
        ArticleCollection w;
        w.add(new Article("King"));
        CHECK(w.get("Queen") == nullptr);

    }
}
