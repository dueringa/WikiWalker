#include <UnitTest++.h>

#include <algorithm>

#include "../src/Article.h"
#include "../src/WalkerException.h"

SUITE(ArticleTests)
{
    TEST(Article_GetNumLinks_Uninited_ThrowsException)
    {
        Article a("Foo");
        CHECK_EQUAL(a.isAnalyzed(), false);
        CHECK_THROW(a.getNumLinks(), WalkerException);

    }

    TEST(Article_GetNumLinks_Inited_ReturnsNull)
    {
        Article a("Foo");
        a.setAnalyzed(true);
        CHECK_EQUAL(a.isAnalyzed(), true);
        CHECK_EQUAL(0, a.getNumLinks());
    }

    TEST(Article_AddLinks_OneLinkAdded)
    {
        Article a("Foo");

        a.addLink(new Article("Barmiz"));

        CHECK_EQUAL(a.isAnalyzed(), true);
        CHECK_EQUAL(1, a.getNumLinks());
    }

    TEST(Article_AddLinks_DuplicateInstance)
    {
        Article a("Foo");

        auto arl = new Article("Barmiz");
        CHECK(a.addLink(arl));
        CHECK(!a.addLink(arl));

        CHECK_EQUAL(a.isAnalyzed(), true);
        CHECK_EQUAL(1, a.getNumLinks());
    }

    TEST(Article_Iterator_Test)
    {
        Article a("Foo");

        std::vector<std::string> titles { "Barmiz", "Kodopa", "Minting" };

        for(auto s : titles) {
            a.addLink(new Article(s));
        }

        CHECK_EQUAL(a.isAnalyzed(), true);
        CHECK_EQUAL(3, a.getNumLinks());

        int num = 0;
        for(auto x = a.linkBegin(); x != a.linkEnd(); x++) {
            num++;
            auto atitle = (*x)->getTitle();
            auto findpos = std::find(titles.cbegin(), titles.cend(), atitle);
            bool isFound = (titles.end() != findpos);
            CHECK(isFound);
        }

        CHECK_EQUAL(titles.size(), num);
    }

    TEST(Article_Marked_State)
    {
        Article a("Todo");
        CHECK_EQUAL(false, a.isMarked());
        a.setMarked(true);
        CHECK_EQUAL(true, a.isMarked());
        a.setMarked(false);
        CHECK_EQUAL(false, a.isMarked());
    }
}
