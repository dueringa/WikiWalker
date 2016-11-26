#include <UnitTest++.h>

#include "../src/ToJsonWriter.h"
#include "../src/Article.h"

SUITE(ArticleToJsonWriterTests)
{
    TEST(WriteUnanalyzedArticleWithoutLinks_LinksIsNull)
    {
        ToJsonWriter atj;
        Article a("Farm");

        CHECK_EQUAL("{\"Farm\":{\"forward_links\":null}}", atj.convertToJson(&a));
    }

    TEST(WriteAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
    {
        ToJsonWriter atj;
        Article a("Farm");
        a.setAnalyzed(true);

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

    TEST(WriteArticleCollection_OneUnanalyzedArticleWithoutLinks_LinksIsNull)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        ac.add(new Article("Foo"));
        CHECK_EQUAL("{\"Foo\":{\"forward_links\":null}}", atj.convertToJson(ac));
    }
    TEST(WriteArticleCollection_OneAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        auto a = new Article("Foo");
        a->setAnalyzed(true);
        ac.add(a);
        CHECK_EQUAL("{\"Foo\":{\"forward_links\":[]}}", atj.convertToJson(ac));
    }
}
