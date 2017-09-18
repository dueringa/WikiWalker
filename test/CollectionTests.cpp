#include <memory>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "ArticleCollection.h"
#include "WalkerException.h"

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

  // dragon -> treasure
  //        -> fire
  //        -> flying
  // cat    -> -
  // apple  -> fruit
  // window -> outside
  void createArticlesAndFillFirst(ArticleCollection & ac)
  {
    auto a1 = std::make_shared<Article>("Dragon");
    auto a2 = std::make_shared<Article>("Treasure");
    auto a3 = std::make_shared<Article>("Fire");
    auto a4 = std::make_shared<Article>("Flying");
    a1->addLink(a2);
    a1->addLink(a3);
    a1->addLink(a4);

    auto a5 = std::make_shared<Article>("Cat");

    auto a6 = std::make_shared<Article>("Apple");
    auto a7 = std::make_shared<Article>("Fruit");
    a6->addLink(a7);

    auto a8 = std::make_shared<Article>("Window");
    auto a9 = std::make_shared<Article>("Outside");
    a8->addLink(a9);

    ac.add(a1);
    ac.add(a2);
    ac.add(a3);
    ac.add(a4);
    ac.add(a5);
    ac.add(a6);
    ac.add(a7);
    ac.add(a8);
    ac.add(a9);
  }

  // dragon -> -
  // cat    -> milk
  //        -> lazy
  // wood   -> house
  // window -> glass
  //        -> cleaning
  void createArticlesAndFillSecond(ArticleCollection & ac)
  {
    auto b1 = std::make_shared<Article>("Dragon");

    auto b2 = std::make_shared<Article>("Cat");
    auto b9 = std::make_shared<Article>("Milk");
    auto b3 = std::make_shared<Article>("Lazy");
    b2->addLink(b3);
    b2->addLink(b9);

    auto b4 = std::make_shared<Article>("Wood");
    auto b5 = std::make_shared<Article>("House");
    b4->addLink(b5);

    auto b6 = std::make_shared<Article>("Window");
    auto b7 = std::make_shared<Article>("Glass");
    auto b8 = std::make_shared<Article>("Cleaning");
    b6->addLink(b7);
    b6->addLink(b8);

    ac.add(b1);
    ac.add(b2);
    ac.add(b3);
    ac.add(b4);
    ac.add(b5);
    ac.add(b6);
    ac.add(b7);
    ac.add(b8);
    ac.add(b9);
  }

  TEST(ArticleCollection_TestMergeIgnore)
  {
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      a1.merge(a2, ArticleCollection::MergeStrategy::IgnoreDuplicates);

      CHECK_EQUAL(15, a1.getNumArticles());

      auto ptr = a1.get("Dragon");
      CHECK(ptr != nullptr);
      CHECK_EQUAL(3, ptr->getNumLinks());

      ptr = a1.get("Cat");
      CHECK(ptr != nullptr);
      CHECK_THROW(ptr->getNumLinks(), WalkerException);

      ptr = a1.get("Window");
      CHECK(ptr != nullptr);
      CHECK_EQUAL(1, ptr->getNumLinks());
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      a2.merge(a1, ArticleCollection::MergeStrategy::IgnoreDuplicates);
      CHECK_EQUAL(15, a2.getNumArticles());

      auto ptr = a2.get("Dragon");
      CHECK(ptr != nullptr);
      CHECK_THROW(ptr->getNumLinks(), WalkerException);

      ptr = a2.get("Cat");
      CHECK(ptr != nullptr);
      CHECK_EQUAL(2, ptr->getNumLinks());

      ptr = a2.get("Window");
      CHECK(ptr != nullptr);
      CHECK_EQUAL(2, ptr->getNumLinks());
    }
  }

  TEST(ArticleCollection_TestMerge)
  {
    ArticleCollection ac1;
    ac1.add(std::make_shared<Article>("ManaMana"));
    ac1.add(std::make_shared<Article>("Dragon"));
    ac1.add(std::make_shared<Article>("Cereals"));

    {
      ArticleCollection ac2;
      ac2.add(std::make_shared<Article>("Dragon"));
      ac2.add(std::make_shared<Article>("Git"));
      ac2.add(std::make_shared<Article>("Stroustrup"));
      ac1.merge(ac2, ArticleCollection::MergeStrategy::IgnoreDuplicates);

      CHECK_EQUAL(5, ac1.getNumArticles());
      CHECK_EQUAL(3, ac2.getNumArticles());
    }
    // check again after scope is left
    CHECK_EQUAL(5, ac1.getNumArticles());
  }
}
