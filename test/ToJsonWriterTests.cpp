#include <UnitTest++.h>

#include "../src/ToJsonWriter.h"

SUITE(ArticleToJsonWriterTests)
{
    TEST(WriteArticleWithoutLinks)
    {
        ToJsonWriter atj;
        Article a("Farm");
        CHECK_EQUAL("{\"Farm\":[]}", atj.convertToJson(&a));
    }

    TEST(WriteArticleWithOneLink)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        CHECK_EQUAL("{\"Farm\":[\"Animal\"]}", atj.convertToJson(&a));
    }

    TEST(WriteArticleWithMultipleLinks)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        a.addLink(new Article("Pig"));
        a.addLink(new Article("Equality"));
        CHECK_EQUAL("{\"Farm\":[\"Animal\",\"Pig\",\"Equality\"]}", atj.convertToJson(&a));
    }
}
