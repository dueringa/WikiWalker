#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <sstream>
#include <utility>

#include "Article.h"
#include "JsonSerializer.h"

SUITE(JsonSerializeDeserializeTests)
{
  using namespace WikiWalker;

  TEST(SerializeUnanalyzedArticleWithoutLinks_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      ac.add(std::make_shared<Article>("Farm"));
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(1, ac2.countArticles());
    CHECK(ac2.get("Farm") != nullptr);
    CHECK(!ac2.get("Farm")->analyzed());
  }

  TEST(SerializeAnalyzedArticleWithoutLinks_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      auto a = std::make_shared<Article>("Farm");
      ac.add(a);
      a->analyzed(true);
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(1, ac2.countArticles());
    CHECK(ac2.get("Farm") != nullptr);
    CHECK(ac2.get("Farm")->analyzed());
    CHECK_EQUAL(0, ac2.get("Farm")->numLinks());
  }

  TEST(SerializeArticleWithOneLink_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      // yes, only a is inserted, since we want to emulate article-only
      auto a = std::make_shared<Article>("Farm");
      ac.add(a);
      CHECK_EQUAL(1, ac.countArticles());
      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
      CHECK_EQUAL(1, ac.countArticles());
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    // well, whoops, what is correct now?
    CHECK_EQUAL(1, ac2.countArticles());
    CHECK(ac2.get("Farm") != nullptr);
    CHECK(ac2.get("Farm")->analyzed());
    CHECK_EQUAL(1, ac2.get("Farm")->numLinks());
  }

  TEST(SerializeArticleWithMultipleLinks_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;

      // yes, only a is inserted, since we want to emulate article-only
      auto a = std::make_shared<Article>("Farm");
      ac.add(a);
      CHECK_EQUAL(1, ac.countArticles());

      auto al1 = std::make_shared<Article>("Animal"),
           al2 = std::make_shared<Article>("Pig"),
           al3 = std::make_shared<Article>("Equality");
      a->addLink(al1);
      a->addLink(al2);
      a->addLink(al3);
      CHECK_EQUAL(1, ac.countArticles());
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    // well, whoops, what is correct now?
    CHECK_EQUAL(1, ac2.countArticles());
    CHECK(ac2.get("Farm") != nullptr);
    CHECK(ac2.get("Farm")->analyzed());
    CHECK_EQUAL(3, ac2.get("Farm")->numLinks());
  }

  TEST(SerializeEmptyArticleCollection_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(0, ac2.countArticles());
  }

  TEST(
      SerializeArticleCollection_OneUnanalyzedArticleWithoutLinks_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      auto linked = std::make_shared<Article>("Foo");
      ac.add(linked);
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(1, ac2.countArticles());
    CHECK(ac2.get("Foo") != nullptr);
    CHECK(!ac2.get("Foo")->analyzed());
  }

  TEST(SerializeArticleCollection_OneAnalyzedArticleWithoutLinks_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      auto a = std::make_shared<Article>("Foo");
      a->analyzed(true);
      ac.add(a);
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(1, ac2.countArticles());
    CHECK(ac2.get("Foo") != nullptr);
    CHECK(ac2.get("Foo")->analyzed());
    CHECK_EQUAL(0, ac2.get("Foo")->numLinks());
  }
}
