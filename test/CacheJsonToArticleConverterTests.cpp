#include <UnitTest++.h>

#include "../src/CacheJsonToArticleConverter.h"
#include "../src/Article.h"

SUITE(CacheJsonToArticleConverterTests)
{
    TEST(GetArticleWithoutLinks)
    {
        std::string json = "{\"Farm\":{\"forward_links\":[]}}";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(1, ac.getNumArticles());

        Article* a = ac.get("Farm");
        CHECK(a != nullptr);

        // true or false, that's the question...
        // CHECK_EQUAL(???, a->isAnalyzed());
        // crashes
        // CHECK_EQUAL(0, a->getNumLinks());
    }

    TEST(GetArticleWithOneLink)
    {
        std::string json = "{\"Farm\":{\"forward_links\":[\"Animal\"]}}";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(2, ac.getNumArticles());

        Article* a = ac.get("Farm");
        CHECK(a != nullptr);

        CHECK_EQUAL(1, a->getNumLinks());
    }

    TEST(GetArticleWithMultipleLinks)
    {
        std::string json = "{\"Farm\":{\"forward_links\":[\"Animal\",\"Pig\",\"Equality\"]}}";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(4, ac.getNumArticles());

        Article* a = ac.get("Farm");
        CHECK(a != nullptr);

        CHECK_EQUAL(3, a->getNumLinks());
    }

    TEST(WriteEmptyArticleCollection)
    {
        std::string json = "{}";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);
        CHECK_EQUAL(0, ac.getNumArticles());
    }

    TEST(WriteArticleCollection_OneArticleWithoutLinks)
    {
        std::string json = "{\"Foo\":{\"forward_links\":[]}}";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(1, ac.getNumArticles());

        Article* a = ac.get("Foo");
        CHECK(a != nullptr);
    }
}
