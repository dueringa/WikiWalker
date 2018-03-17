#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <sstream>
#include <utility>

#include "Article.h"
#include "JsonSerializer.h"

SUITE(ArticleJsonSerializerTests)
{
  using namespace WikiWalker;

  TEST(WriteUnanalyzedArticleWithoutLinks_LinksIsNull)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;
    ac.add(std::make_shared<Article>("Farm"));

    atj.serialize(ac, oss);

    CHECK_EQUAL(R"({"Farm":{"forward_links":null}})", oss.str());
  }

  TEST(WriteAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    auto a = std::make_shared<Article>("Farm");
    ac.add(a);
    a->analyzed(true);

    atj.serialize(ac, oss);

    CHECK_EQUAL(R"({"Farm":{"forward_links":[]}})", oss.str());
  }

  TEST(WriteArticleWithOneLink)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    ac.add(a);

    auto linked = std::make_shared<Article>("Animal");
    a->addLink(linked);

    atj.serialize(ac, oss);
    CHECK_EQUAL(R"({"Farm":{"forward_links":["Animal"]}})", oss.str());
  }

  TEST(WriteArticleWithMultipleLinks)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    ac.add(a);

    auto al1 = std::make_shared<Article>("Animal"),
         al2 = std::make_shared<Article>("Pig"),
         al3 = std::make_shared<Article>("Equality");

    a->addLink(al1);
    a->addLink(al2);
    a->addLink(al3);

    atj.serialize(ac, oss);

    CHECK_EQUAL(
        R"({"Farm":{"forward_links":["Animal","Pig","Equality"]}})",
        oss.str());
  }

  TEST(WriteEmptyArticleCollection)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    atj.serialize(ac, oss);

    CHECK_EQUAL("{}", oss.str());
  }

  TEST(WriteArticleCollection_OneUnanalyzedArticleWithoutLinks_LinksIsNull)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto linked = std::make_shared<Article>("Foo");
    ac.add(linked);

    atj.serialize(ac, oss);

    CHECK_EQUAL(R"({"Foo":{"forward_links":null}})", oss.str());
  }

  TEST(WriteArticleCollection_OneAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto a = std::make_shared<Article>("Foo");
    a->analyzed(true);
    ac.add(a);

    atj.serialize(ac, oss);

    CHECK_EQUAL(R"({"Foo":{"forward_links":[]}})", oss.str());
  }
}
