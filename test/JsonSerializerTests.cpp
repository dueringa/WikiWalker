#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <sstream>
#include <utility>

#include "Article.h"
#include "JsonSerializer.h"

SUITE(ArticleJsonSerializerTests)
{
  using namespace WikiWalker;

#define WW_PROTOCOL_HEADER \
  R"("program":"wikiwalker","scheme-version":2,"ArticleCollection":)"

  TEST(WriteUnanalyzedArticleWithoutLinks_LinksIsNull)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;
    ac.add(std::make_shared<Article>("Farm"));

    atj.serialize(ac, oss);

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Farm":{"forward_links":null})"
                "}"
                "}",
                oss.str());
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

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Farm":{"forward_links":[]})"
                "}"
                "}",
                oss.str());
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
    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Farm":{"forward_links":["Animal"]})"
                "}"
                "}",
                oss.str());
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

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Farm":{"forward_links":["Animal","Pig","Equality"]})"
                "}"
                "}",
                oss.str());
  }

  TEST(WriteEmptyArticleCollection)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    atj.serialize(ac, oss);

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{}"
                "}",
                oss.str());
  }

  TEST(WriteArticleCollection_OneUnanalyzedArticleWithoutLinks_LinksIsNull)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto linked = std::make_shared<Article>("Foo");
    ac.add(linked);

    atj.serialize(ac, oss);

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Foo":{"forward_links":null})"
                "}"
                "}",
                oss.str());
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

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Foo":{"forward_links":[]})"
                "}"
                "}",
                oss.str());
  }

  TEST(
      WriteArticleCollection_MultipleArticles_WithMultipleLinks_MatchesExpected)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto a = std::make_shared<Article>("Foo");
    auto b = std::make_shared<Article>("Bar");
    auto c = std::make_shared<Article>("Baz");
    a->addLink(b);
    b->addLink(a);
    b->addLink(c);
    ac.add(a);
    ac.add(b);
    ac.add(c);

    atj.serialize(ac, oss);

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Bar":{"forward_links":["Foo","Baz"]},)"
                R"("Baz":{"forward_links":null},)"
                R"("Foo":{"forward_links":["Bar"]})"
                "}"
                "}",
                oss.str());
  }

  TEST(SerializeArticleWithOnlyNullptr_NullptrWillBeSkipped)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    ac.add(a);

    {
      // will be skipped on serialization, since it'll become nullptr
      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
    }

    atj.serialize(ac, oss);

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Farm":{"forward_links":[]})"
                "}"
                "}",
                oss.str());
  }

  TEST(SerializeArticleWithValidArticleAndANullptr_NullptrWillBeSkipped)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    ac.add(a);

    {
      // will be skipped on serialization, since it'll become nullptr
      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
    }

    auto linked2 = std::make_shared<Article>("Barn");
    a->addLink(linked2);

    atj.serialize(ac, oss);

    CHECK_EQUAL("{" WW_PROTOCOL_HEADER
                "{"
                R"("Farm":{"forward_links":["Barn"]})"
                "}"
                "}",
                oss.str());
  }
}
