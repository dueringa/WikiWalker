#include <UnitTest++/UnitTest++.h>

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

#include "Article.h"
#include "WalkerException.h"

SUITE(ArticleTests)
{
  using namespace WikiWalker;

  TEST(Article_GetCountLinks_Uninited_ThrowsException)
  {
    Article a("Foo");
    CHECK_EQUAL(a.analyzed(), false);
    CHECK_THROW(a.countLinks(), WalkerException);
  }

  TEST(Article_GetCountLinks_Inited_ReturnsNull)
  {
    Article a("Foo");
    a.analyzed(true);
    CHECK_EQUAL(a.analyzed(), true);
    CHECK_EQUAL(0, a.countLinks());
  }

  TEST(Article_AddLinks_OneLinkAdded)
  {
    Article a("Foo");

    auto link = std::make_shared<Article>("Barmiz");
    a.addLink(link);

    CHECK_EQUAL(a.analyzed(), true);
    CHECK_EQUAL(1, a.countLinks());
  }

  TEST(Article_AddLinks_DuplicateInstance)
  {
    Article a("Foo");

    auto arl = std::make_shared<Article>("Barmiz");
    CHECK(a.addLink(arl));
    CHECK(!a.addLink(arl));

    CHECK_EQUAL(a.analyzed(), true);
    CHECK_EQUAL(1, a.countLinks());
  }

  TEST(Article_AddLinks_WhenLinkExpiredToNulltpr)
  {
    Article a("Foo");

    {
      /* Checks for absence of bug with nullptr access when checking for
       * duplicates */
      auto arl = std::make_shared<Article>("Barmiz");
      CHECK(a.addLink(arl));
    }

    auto arl2 = std::make_shared<Article>("Barmiz");
    CHECK(a.addLink(arl2));

    CHECK_EQUAL(a.analyzed(), true);
    /* this one's actually awkward and is counterintuitive. But since the
     * shared_ptr went out of scope, it expired. I chose to simply ignore
     * expired smart pointers / nullptrs inside the link collection for actual
     * processing purposes. They're still counted, however. */
    CHECK_EQUAL(2, a.countLinks());
  }

  TEST(Article_Iterator_Test)
  {
    Article a("Foo");

    std::vector<std::string> titles{"Barmiz", "Kodopa", "Minting"};
    // must keep them in scope
    std::vector<std::shared_ptr<Article>> articleLinks;

    std::transform(titles.begin(),
                   titles.end(),
                   std::back_inserter(articleLinks),
                   [](std::string s) { return std::make_shared<Article>(s); });

    for(const auto& s : articleLinks) {
      a.addLink(s);
    }

    CHECK_EQUAL(a.analyzed(), true);
    CHECK_EQUAL(3, a.countLinks());

    int num = 0;
    for(auto x = a.linkBegin(); x != a.linkEnd(); x++) {
      num++;
      auto atitle  = x->lock()->title();
      auto findpos = std::find(titles.cbegin(), titles.cend(), atitle);
      bool isFound = (titles.end() != findpos);
      CHECK(isFound);
    }

    CHECK_EQUAL(titles.size(), num);
  }

  TEST(Article_Marked_State)
  {
    Article a("Todo");
    CHECK_EQUAL(false, a.marked());
    a.marked(true);
    CHECK_EQUAL(true, a.marked());
    a.marked(false);
    CHECK_EQUAL(false, a.marked());
  }
}
