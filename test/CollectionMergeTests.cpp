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
    CHECK_EQUAL(15, c1.countArticles());

    // data from createArticlesAndFillFirst won
    auto ptr = c1.get("Dragon");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(3, ptr->countLinks());

    ptr = c1.get("Cat");
    CHECK(ptr != nullptr);
    CHECK_THROW(ptr->countLinks(), WalkerException);

    ptr = c1.get("Window");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(1, ptr->countLinks());
  }

  /*!
   * see #checkConflicts_DataFromFirstSetPreferred, only for the second set
   */
  void checkConflicts_DataFromSecondSetPreferred(ArticleCollection & c2)
  {
    CHECK_EQUAL(15, c2.countArticles());

    // data from createArticlesAndFillSecond won
    auto ptr = c2.get("Dragon");
    CHECK(ptr != nullptr);
    CHECK_THROW(ptr->countLinks(), WalkerException);

    ptr = c2.get("Cat");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());

    ptr = c2.get("Window");
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
    auto ptr = c.get("Apple");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(1, ptr->countLinks());

    ptr = c.get("Wood");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(1, ptr->countLinks());
  }

  TEST(ArticleCollection_TestMergeIgnore)
  {
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      a1.merge(a2, CollectionUtils::MergeStrategy::IgnoreDuplicates);
      checkConflicts_DataFromFirstSetPreferred(a1);
      checkNonConflictingItems(a1);
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      a2.merge(a1, CollectionUtils::MergeStrategy::IgnoreDuplicates);
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
      a1.merge(a2, CollectionUtils::MergeStrategy::AlwaysOverwrite);
      checkConflicts_DataFromSecondSetPreferred(a1);
      checkNonConflictingItems(a1);
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      a2.merge(a1, CollectionUtils::MergeStrategy::AlwaysOverwrite);
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
    auto ptr = ac.get("Dragon");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(3, ptr->countLinks());

    ptr = ac.get("Cat");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());

    ptr = ac.get("Window");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(2, ptr->countLinks());
  }

  TEST(ArticleCollection_TestMergeMoreLinks)
  {
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      a1.merge(a2, CollectionUtils::MergeStrategy::UseArticleWithMoreLinks);
      CHECK_EQUAL(15, a1.countArticles());

      checkConflicts_DataWithMoreLinksPreferred(a1);
      checkNonConflictingItems(a1);
    }
    {
      ArticleCollection a1, a2;
      createArticlesAndFillFirst(a1);
      createArticlesAndFillSecond(a2);
      // reverse merge
      a2.merge(a1, CollectionUtils::MergeStrategy::UseArticleWithMoreLinks);
      CHECK_EQUAL(15, a2.countArticles());

      checkConflicts_DataWithMoreLinksPreferred(a2);
      checkNonConflictingItems(a2);
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
      ac1.merge(ac2, CollectionUtils::MergeStrategy::IgnoreDuplicates);

      CHECK_EQUAL(5, ac1.countArticles());
      CHECK_EQUAL(3, ac2.countArticles());
    }
    // check again after scope is left
    CHECK_EQUAL(5, ac1.countArticles());
  }
}
