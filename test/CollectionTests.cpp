#include <memory>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "ArticleCollection.h"

SUITE(CollectionTests)
{
  using namespace WikiWalker;

  TEST(MakeSureNoDuplicateArticlesExist)
  {
    ArticleCollection w;
    auto la1 = std::make_shared<Article>("King"),
         la2 = std::make_shared<Article>("Queen"),
         la3 = std::make_shared<Article>("Prince"),
         la4 = std::make_shared<Article>("Queen");

    CHECK(CollectionUtils::add(w, la1));
    CHECK(CollectionUtils::add(w, la2));
    CHECK_EQUAL(2, w.count());
    CHECK(CollectionUtils::add(w, la3));
    CHECK_EQUAL(3, w.count());

    // must fail
    CHECK(!CollectionUtils::add(w, la4));
    CHECK_EQUAL(3, w.count());
  }

  TEST(CollectionIsCaseInsensitive)
  {
    ArticleCollection w;
    auto la1 = std::make_shared<Article>("King"),
         la2 = std::make_shared<Article>("Queen"),
         la3 = std::make_shared<Article>("Prince"),
         la4 = std::make_shared<Article>("queen");

    CollectionUtils::add(w, la1);
    CollectionUtils::add(w, la2);
    CHECK_EQUAL(2, w.count());
    CollectionUtils::add(w, la3);
    CHECK_EQUAL(3, w.count());
    CollectionUtils::add(w, la4);
    CHECK_EQUAL(4, w.count());
  }

  TEST(GetArticle_Existing_MustNotBeNull)
  {
    ArticleCollection w;
    auto king = std::make_shared<Article>("King");
    CollectionUtils::add(w, king);
    CHECK(w.get("King") != nullptr);
  }

  TEST(GetArticle_NonExisting_MustBeNull)
  {
    ArticleCollection w;
    auto la1 = std::make_shared<Article>("King");
    CollectionUtils::add(w, la1);
    CHECK(w.get("Queen") == nullptr);
  }

  ArticleCollection GetArticleCollection()
  {
    ArticleCollection ac;
    CollectionUtils::add(ac, std::make_shared<Article>("Foo"));
    CollectionUtils::add(ac, std::make_shared<Article>("Bar"));
    return ac;
  }

  TEST(ArticleCollection_CreationViaMoveConstructor)
  {
    auto ac = GetArticleCollection();
    CHECK_EQUAL(2, ac.count());
    CHECK(ac.get("Foo") != nullptr);
  }
}
