#include <UnitTest++.h>

#include "../src/ToJsonWriter.h"

SUITE(ArticleToJsonWriterTests)
{
    TEST(Unidentified)
    {
        ToJsonWriter atj;
        Article a("Farm");
        CHECK_EQUAL("", atj.convertToJson(&a));
    }

    TEST(Unidentified2)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        CHECK_EQUAL("", atj.convertToJson(&a));
    }

    TEST(Unidentified3)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        a.addLink(new Article("Pig"));
        a.addLink(new Article("Equality"));
        CHECK_EQUAL("", atj.convertToJson(&a));
    }
}
