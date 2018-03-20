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
      CollectionUtils::add(ac, std::make_shared<Article>("Farm"));
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(1, ac2.size());
    CHECK(CollectionUtils::get(ac2, "Farm") != nullptr);
    CHECK(!CollectionUtils::get(ac2, "Farm")->analyzed());
  }

  TEST(SerializeAnalyzedArticleWithoutLinks_DeserializesInSameState)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      auto a = std::make_shared<Article>("Farm");
      CollectionUtils::add(ac, a);
      a->analyzed(true);
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);
    CHECK_EQUAL(1, ac2.size());
    CHECK(CollectionUtils::get(ac2, "Farm") != nullptr);
    CHECK(CollectionUtils::get(ac2, "Farm")->analyzed());
    CHECK_EQUAL(0, CollectionUtils::get(ac2, "Farm")->countLinks());
  }

  TEST(SerializeArticleWithOneLink_DeserializesTwoArticles)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;
      // yes, only a is inserted, since we want to emulate article-only
      auto a = std::make_shared<Article>("Farm");
      CollectionUtils::add(ac, a);

      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);

    // Per design, linked-only articles are restored as "new" ones
    CHECK_EQUAL(2, ac2.size());

    CHECK(CollectionUtils::get(ac2, "Farm") != nullptr);
    CHECK(CollectionUtils::get(ac2, "Farm")->analyzed());
    CHECK_EQUAL(1, CollectionUtils::get(ac2, "Farm")->countLinks());

    CHECK(CollectionUtils::get(ac2, "Animal") != nullptr);
    CHECK(!CollectionUtils::get(ac2, "Animal")->analyzed());
  }

  TEST(SerializeArticleWithMultipleLinks_DeserializesNewArticles)
  {
    JsonSerializer atj;
    std::stringstream ss;

    {
      ArticleCollection ac;

      // yes, only a is inserted, since we want to emulate article-only
      auto a = std::make_shared<Article>("Farm");
      CollectionUtils::add(ac, a);
      CHECK_EQUAL(1, ac.size());

      auto al1 = std::make_shared<Article>("Animal"),
           al2 = std::make_shared<Article>("Pig"),
           al3 = std::make_shared<Article>("Equality");
      a->addLink(al1);
      a->addLink(al2);
      a->addLink(al3);
      CHECK_EQUAL(1, ac.size());
      atj.serialize(ac, ss);
    }

    ArticleCollection ac2;
    atj.deserialize(ac2, ss);

    // Per design, linked-only articles are restored as "new" ones
    CHECK_EQUAL(4, ac2.size());

    CHECK(CollectionUtils::get(ac2, "Farm") != nullptr);
    CHECK(CollectionUtils::get(ac2, "Farm")->analyzed());
    CHECK_EQUAL(3, CollectionUtils::get(ac2, "Farm")->countLinks());

    for(std::string title : {"Animal", "Pig", "Equality"}) {
      CHECK(CollectionUtils::get(ac2, title) != nullptr);
      CHECK(!CollectionUtils::get(ac2, title)->analyzed());
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
    CHECK_EQUAL(0, ac2.size());
  }
}
