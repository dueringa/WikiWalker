#include <memory>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "CacheJsonToArticleConverter.h"

SUITE(CacheJsonToArticleConverterTests)
{
  using namespace WikiWalker;

  TEST(GetArticleWithoutLinks_Unanalyzed)
  {
    std::string json = R"({"Farm":{"forward_links":null}})";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);
    CHECK_EQUAL(false, a->analyzed());
  }

  TEST(GetArticleWithoutLinks_Analyzed)
  {
    std::string json = R"({"Farm":{"forward_links":[]}})";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(true, a->analyzed());
    CHECK_EQUAL(0, a->numLinks());
  }

  TEST(GetArticleWithOneLink)
  {
    std::string json = R"({"Farm":{"forward_links":["Animal"]}})";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);

    CHECK_EQUAL(2, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(1, a->numLinks());
  }

  TEST(GetArticleWithMultipleLinks)
  {
    std::string json =
        R"({"Farm":{"forward_links":["Animal","Pig","Equality"]}})";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);

    CHECK_EQUAL(4, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(3, a->numLinks());
  }

  TEST(WriteEmptyArticleCollection)
  {
    std::string json = "{}";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);
    CHECK_EQUAL(0, ac.countArticles());
  }

  TEST(WriteArticleCollection_OneArticleWithoutLinks_Unanalyzed)
  {
    std::string json = R"({"Foo":{"forward_links":null}})";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Foo");
    CHECK(a != nullptr);
    CHECK_EQUAL(false, a->analyzed());
  }

  TEST(WriteArticleCollection_OneArticleWithoutLinks_Analyzed)
  {
    std::string json = R"({"Foo":{"forward_links":[]}})";

    ArticleCollection ac;
    CacheJsonToArticleConverter cjta;
    cjta.convert(json, ac);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Foo");
    CHECK(a != nullptr);
    CHECK_EQUAL(true, a->analyzed());
    CHECK_EQUAL(0, a->numLinks());
  }
}
