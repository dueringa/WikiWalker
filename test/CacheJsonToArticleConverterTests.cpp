#include <UnitTest++/UnitTest++.h>

#include "../src/CacheJsonToArticleConverter.h"
#include "../src/Article.h"

#include <memory>

SUITE(CacheJsonToArticleConverterTests)
{
    TEST(GetArticleWithoutLinks_Unanalyzed)
    {
        std::string json = R"({"Farm":{"forward_links":null}})";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(1, ac.getNumArticles());

        auto a = ac.get("Farm");
        CHECK(a != nullptr);
        CHECK_EQUAL(false, a->isAnalyzed());
    }

    TEST(GetArticleWithoutLinks_Analyzed)
    {
        std::string json = R"({"Farm":{"forward_links":[]}})";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(1, ac.getNumArticles());

        auto a = ac.get("Farm");
        CHECK(a != nullptr);

        CHECK_EQUAL(true, a->isAnalyzed());
        CHECK_EQUAL(0, a->getNumLinks());
    }

    TEST(GetArticleWithOneLink)
    {
        std::string json = R"({"Farm":{"forward_links":["Animal"]}})";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(2, ac.getNumArticles());

        auto a = ac.get("Farm");
        CHECK(a != nullptr);

        CHECK_EQUAL(1, a->getNumLinks());
    }

    TEST(GetArticleWithMultipleLinks)
    {
        std::string json = R"({"Farm":{"forward_links":["Animal","Pig","Equality"]}})";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(4, ac.getNumArticles());

        auto a = ac.get("Farm");
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

    TEST(WriteArticleCollection_OneArticleWithoutLinks_Unanalyzed)
    {
        std::string json = R"({"Foo":{"forward_links":null}})";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(1, ac.getNumArticles());

        auto a = ac.get("Foo");
        CHECK(a != nullptr);
        CHECK_EQUAL(false, a->isAnalyzed());
    }

    TEST(WriteArticleCollection_OneArticleWithoutLinks_Analyzed)
    {
        std::string json = R"({"Foo":{"forward_links":[]}})";

        ArticleCollection ac;
        CacheJsonToArticleConverter cjta;
        cjta.convertToArticle(json, ac);

        CHECK_EQUAL(1, ac.getNumArticles());

        auto a = ac.get("Foo");
        CHECK(a != nullptr);
        CHECK_EQUAL(true, a->isAnalyzed());
        CHECK_EQUAL(0, a->getNumLinks());
    }
}
