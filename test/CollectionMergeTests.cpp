#include <memory>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "ArticleCollection.h"
#include "WalkerException.h"

SUITE(CollectionMergeTests)
{
  using namespace WikiWalker;

  /*!
   * Create test data of the following structure:
   * dragon -> treasure
   *        -> fire
   *        -> flying
   * cat    -> -
   * apple  -> fruit
   * window -> outside
   */
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

    CollectionUtils::add(ac, a1);
    CollectionUtils::add(ac, a2);
    CollectionUtils::add(ac, a3);
    CollectionUtils::add(ac, a4);
    CollectionUtils::add(ac, a5);
    CollectionUtils::add(ac, a6);
    CollectionUtils::add(ac, a7);
    CollectionUtils::add(ac, a8);
    CollectionUtils::add(ac, a9);
  }

  /*!
   * Create test data of the following structure:
   *
   * dragon -> -
   * cat    -> milk
   *        -> lazy
   * wood   -> house
   * window -> glass
   *        -> cleaning
   */
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

    CollectionUtils::add(ac, b1);
    CollectionUtils::add(ac, b2);
    CollectionUtils::add(ac, b3);
    CollectionUtils::add(ac, b4);
    CollectionUtils::add(ac, b5);
    CollectionUtils::add(ac, b6);
    CollectionUtils::add(ac, b7);
    CollectionUtils::add(ac, b8);
    CollectionUtils::add(ac, b9);
  }

  /*! check whether data from first set is preferred
   *
   * Overview of the combined structure-
   *
   * dragon -> treasure | dragon -> -
   *        -> fire     |
   *        -> flying   |
   *                    |
   * cat    -> -        | cat    -> milk
   *                    |        -> lazy
   *                    |
   * apple  -> fruit    |
   *                    |
   *                    | wood   -> house
   *                    |
   * window -> outside  | window -> glass
   *                    |        -> cleaning
   *
   *
   * So we have 15 articles in total, and either side of the links may exist
   */
  void checkConflicts_DataFromFirstSetPreferred(ArticleCollection & c1)
  {
    // 15 articles in total, no matter what
    CHECK_EQUAL(15, c1.count());

    // data from createArticlesAndFillFirst won
    auto ptr = CollectionUtils::get(c1, "Dragon");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(3, ptr->countLinks());

    ptr = CollectionUtils::get(c1, "Cat");
    CHECK(ptr != nullptr);
    CHECK_THROW(ptr->countLinks(), WalkerException);

    ptr = CollectionUtils::get(c1, "Window");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(1, ptr->countLinks());
  }

  /*!
   * see #checkConflicts_DataFromFirstSetPreferred, only for the second set
   */
  void checkConflicts_DataFromSecondSetPreferred(ArticleCollection & c2)
  {
    CHECK_EQUAL(15, c2.count());

    // data from createArticlesAndFillSecond won
    auto ptr = CollectionUtils::get(c2, "Dragon");
    CHECK(ptr != nullptr);
    CHECK_THROW(ptr->countLinks(), WalkerException);

    ptr = CollectionUtils::get(c2, "Cat");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());

    ptr = CollectionUtils::get(c2, "Window");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());
  }

  /*!
   * see #checkConflicts_DataFromFirstSetPreferred, only for non-conflicting
   * items
   */
  void checkNonConflictingItems(ArticleCollection & c)
  {
    // check non-conflicting items, too
    auto ptr = CollectionUtils::get(c, "Apple");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(1, ptr->countLinks());

    ptr = CollectionUtils::get(c, "Wood");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(1, ptr->countLinks());
  }

  TEST(ArticleCollection_TestMergeIgnore)
  {
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      CollectionUtils::merge(
          a1, a2, CollectionUtils::MergeStrategy::IgnoreDuplicates);
      checkConflicts_DataFromFirstSetPreferred(a1);
      checkNonConflictingItems(a1);
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      CollectionUtils::merge(
          a2, a1, CollectionUtils::MergeStrategy::IgnoreDuplicates);
      checkConflicts_DataFromSecondSetPreferred(a2);
      checkNonConflictingItems(a2);
    }
  }

  // overwrite behaves "exactly the opposite" of ignore
  TEST(ArticleCollection_TestMergeOverwrite)
  {
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      CollectionUtils::merge(
          a1, a2, CollectionUtils::MergeStrategy::AlwaysOverwrite);
      checkConflicts_DataFromSecondSetPreferred(a1);
      checkNonConflictingItems(a1);
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      CollectionUtils::merge(
          a2, a1, CollectionUtils::MergeStrategy::AlwaysOverwrite);
      checkConflicts_DataFromFirstSetPreferred(a2);
      checkNonConflictingItems(a2);
    }
  }

  /*!
   * see #checkConflicts_DataFromFirstSetPreferred, only for items with most
   * links
   */
  void checkConflicts_DataWithMoreLinksPreferred(ArticleCollection & ac)
  {
    auto ptr = CollectionUtils::get(ac, "Dragon");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(3, ptr->countLinks());

    ptr = CollectionUtils::get(ac, "Cat");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());

    ptr = CollectionUtils::get(ac, "Window");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());
  }

  TEST(ArticleCollection_TestMergeMoreLinks)
  {
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      CollectionUtils::merge(
          a1, a2, CollectionUtils::MergeStrategy::UseArticleWithMoreLinks);
      CHECK_EQUAL(15, a1.count());

      checkConflicts_DataWithMoreLinksPreferred(a1);
      checkNonConflictingItems(a1);
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      CollectionUtils::merge(
          a2, a1, CollectionUtils::MergeStrategy::UseArticleWithMoreLinks);
      CHECK_EQUAL(15, a2.count());

      checkConflicts_DataWithMoreLinksPreferred(a2);
      checkNonConflictingItems(a2);
    }
  }

  TEST(ArticleCollection_TestMerge)
  {
    ArticleCollection ac1;
    CollectionUtils::add(ac1, std::make_shared<Article>("ManaMana"));
    CollectionUtils::add(ac1, std::make_shared<Article>("Dragon"));
    CollectionUtils::add(ac1, std::make_shared<Article>("Cereals"));

    {
      ArticleCollection ac2;
      CollectionUtils::add(ac2, std::make_shared<Article>("Dragon"));
      CollectionUtils::add(ac2, std::make_shared<Article>("Git"));
      CollectionUtils::add(ac2, std::make_shared<Article>("Stroustrup"));
      CollectionUtils::merge(
          ac1, ac2, CollectionUtils::MergeStrategy::IgnoreDuplicates);

      CHECK_EQUAL(5, ac1.count());
      CHECK_EQUAL(3, ac2.count());
    }
    // check again after scope is left
    CHECK_EQUAL(5, ac1.count());
  }
}
