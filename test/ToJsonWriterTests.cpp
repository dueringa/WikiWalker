#include <UnitTest++/UnitTest++.h>

#include "../src/ToJsonWriter.h"
#include "../src/Article.h"

#include <memory>
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

        auto linked = std::make_shared<Article>("Animal");
        a.addLink(linked);

        atj.output(&a, oss);
        CHECK_EQUAL("{\"Farm\":{\"forward_links\":[\"Animal\"]}}", oss.str());
    }

    TEST(WriteArticleWithMultipleLinks)
    {
        ToJsonWriter atj;
        Article a("Farm");
        std::ostringstream oss;

        auto al1 = std::make_shared<Article>("Animal"),
            al2 = std::make_shared<Article>("Pig"),
            al3 = std::make_shared<Article>("Equality");

        a.addLink(al1);
        a.addLink(al2);
        a.addLink(al3);

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

        auto linked = std::make_shared<Article>("Foo");
        ac.add(linked);

        atj.output(ac, oss);

        CHECK_EQUAL("{\"Foo\":{\"forward_links\":null}}", oss.str());
    }
    TEST(WriteArticleCollection_OneAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
    {
        ToJsonWriter atj;
        ArticleCollection ac;
        std::ostringstream oss;

        auto a = std::make_shared<Article>("Foo");
        a->setAnalyzed(true);
        ac.add(a);

        atj.output(ac, oss);

        CHECK_EQUAL("{\"Foo\":{\"forward_links\":[]}}", oss.str());
    }
}
