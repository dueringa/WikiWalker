#include <UnitTest++.h>

#include "../src/ToJsonWriter.h"
#include "../src/Article.h"

#include <sstream>

SUITE(ArticleToJsonWriterTests)
{
    TEST(WriteUnanalyzedArticleWithoutLinks_LinksIsNull)
    {
        ToJsonWriter atj;
        Article a("Farm");
        std::ostringstream oss;

        atj.output(&a, oss);

        CHECK_EQUAL("{\"Farm\":{\"forward_links\":null}}", oss.str());
    }

    TEST(WriteAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
    {
        ToJsonWriter atj;
        Article a("Farm");
        std::ostringstream oss;

        a.setAnalyzed(true);

        atj.output(&a, oss);

        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[]}}", oss.str());
    }

    TEST(WriteArticleWithOneLink)
    {
        ToJsonWriter atj;
        Article a("Farm");
        std::ostringstream oss;

        a.addLink(new Article("Animal"));

        atj.output(&a, oss);
        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[\"Animal\"]}}", oss.str());
    }

    TEST(WriteArticleWithMultipleLinks)
    {
        ToJsonWriter atj;
        Article a("Farm");
        std::ostringstream oss;

        a.addLink(new Article("Animal"));
        a.addLink(new Article("Pig"));
        a.addLink(new Article("Equality"));

        atj.output(&a, oss);

        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[\"Animal\",\"Pig\",\"Equality\"]}}",
                    oss.str());
    }

    TEST(WriteEmptyArticleCollection)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        std::ostringstream oss;

        atj.output(ac, oss);

        CHECK_EQUAL("{}", oss.str());
    }

    TEST(WriteArticleCollection_OneUnanalyzedArticleWithoutLinks_LinksIsNull)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        std::ostringstream oss;

        ac.add(new Article("Foo"));

        atj.output(ac, oss);

        CHECK_EQUAL("{\"Foo\":{\"forward_links\":null}}", oss.str());
    }
    TEST(WriteArticleCollection_OneAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        std::ostringstream oss;

        auto a = new Article("Foo");
        a->setAnalyzed(true);
        ac.add(a);

        atj.output(ac, oss);

        CHECK_EQUAL("{\"Foo\":{\"forward_links\":[]}}", oss.str());
    }
}
