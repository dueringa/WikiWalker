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
    CHECK_EQUAL(1, ac2.count());
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
    CHECK_EQUAL(1, ac2.count());
    CHECK(ac2.get("Farm") != nullptr);
    CHECK(ac2.get("Farm")->analyzed());
    CHECK_EQUAL(0, ac2.get("Farm")->countLinks());
  }

  TEST(SerializeArticleWithOneLink_DeserializesTwoArticles)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      // yes, only a is inserted, since we want to emulate article-only
      auto a = std::make_shared<Article>("Farm");
      ac.add(a);

      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);

    // Per design, linked-only articles are restored as "new" ones
    CHECK_EQUAL(2, ac2.count());

    CHECK(ac2.get("Farm") != nullptr);
    CHECK(ac2.get("Farm")->analyzed());
    CHECK_EQUAL(1, ac2.get("Farm")->countLinks());

    CHECK(ac2.get("Animal") != nullptr);
    CHECK(!ac2.get("Animal")->analyzed());
  }

  TEST(SerializeArticleWithMultipleLinks_DeserializesNewArticles)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;

      // yes, only a is inserted, since we want to emulate article-only
      auto a = std::make_shared<Article>("Farm");
      ac.add(a);
      CHECK_EQUAL(1, ac.count());

      auto al1 = std::make_shared<Article>("Animal"),
           al2 = std::make_shared<Article>("Pig"),
           al3 = std::make_shared<Article>("Equality");
      a->addLink(al1);
      a->addLink(al2);
      a->addLink(al3);
      CHECK_EQUAL(1, ac.count());
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);

    // Per design, linked-only articles are restored as "new" ones
    CHECK_EQUAL(4, ac2.count());

    CHECK(ac2.get("Farm") != nullptr);
    CHECK(ac2.get("Farm")->analyzed());
    CHECK_EQUAL(3, ac2.get("Farm")->countLinks());

    for(std::string title : {"Animal", "Pig", "Equality"}) {
      CHECK(ac2.get(title) != nullptr);
      CHECK(!ac2.get(title)->analyzed());
    }
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
    CHECK_EQUAL(0, ac2.count());
  }
}
