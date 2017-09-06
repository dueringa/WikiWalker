#include <memory>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "ArticleCollection.h"

SUITE(CollectionTests)
{
  TEST(MakeSureNoDuplicateArticlesExist)
  {
    ArticleCollection w;
    auto la1 = std::make_shared<Article>("King"),
         la2 = std::make_shared<Article>("Queen"),
         la3 = std::make_shared<Article>("Prince"),
         la4 = std::make_shared<Article>("Queen");

    CHECK(w.add(la1));
    CHECK(w.add(la2));
    CHECK_EQUAL(2, w.getNumArticles());
    CHECK(w.add(la3));
    CHECK_EQUAL(3, w.getNumArticles());

    // must fail
    CHECK(!w.add(la4));
    CHECK_EQUAL(3, w.getNumArticles());
  }

  TEST(CollectionIsCaseInsensitive)
  {
    ArticleCollection w;
    auto la1 = std::make_shared<Article>("King"),
         la2 = std::make_shared<Article>("Queen"),
         la3 = std::make_shared<Article>("Prince"),
         la4 = std::make_shared<Article>("queen");

    w.add(la1);
    w.add(la2);
    CHECK_EQUAL(2, w.getNumArticles());
    w.add(la3);
    CHECK_EQUAL(3, w.getNumArticles());
    w.add(la4);
    CHECK_EQUAL(4, w.getNumArticles());
  }

  TEST(GetArticle_Existing_MustNotBeNull)
  {
    ArticleCollection w;
    auto king = std::make_shared<Article>("King");
    w.add(king);
    CHECK(w.get("King") != nullptr);
  }

  TEST(GetArticle_NonExisting_MustBeNull)
  {
    ArticleCollection w;
    auto la1 = std::make_shared<Article>("King");
    w.add(la1);
    CHECK(w.get("Queen") == nullptr);
  }

  ArticleCollection GetArticleCollection()
  {
    ArticleCollection ac;
    ac.add(std::make_shared<Article>("Foo"));
    ac.add(std::make_shared<Article>("Bar"));
    return ac;
  }

  TEST(ArticleCollection_CreationViaMoveConstructor)
  {
    auto ac = GetArticleCollection();
    CHECK_EQUAL(2, ac.getNumArticles());
    CHECK(ac.get("Foo") != nullptr);
  }
}
