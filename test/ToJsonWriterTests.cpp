#include <UnitTest++.h>

#include "../src/ToJsonWriter.h"
#include "../src/Article.h"

SUITE(ArticleToJsonWriterTests)
{
    TEST(WriteArticleWithoutLinks)
    {
        ToJsonWriter atj;
        Article a("Farm");
        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[]}}", atj.convertToJson(&a));
    }

    TEST(WriteArticleWithOneLink)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[\"Animal\"]}}", atj.convertToJson(&a));
    }

    TEST(WriteArticleWithMultipleLinks)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        a.addLink(new Article("Pig"));
        a.addLink(new Article("Equality"));
        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[\"Animal\",\"Pig\",\"Equality\"]}}", atj.convertToJson(&a));
    }

    TEST(WriteEmptyArticleCollection)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        CHECK_EQUAL("{}", atj.convertToJson(ac));
    }

    TEST(WriteArticleCollection_OneArticleWithoutLinks)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        ac.add(new Article("Foo"));
        CHECK_EQUAL("{\"Foo\":{\"forward_links\":[]}}", atj.convertToJson(ac));
    }
}
