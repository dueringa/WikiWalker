#include <UnitTest++/UnitTest++.h>

#include <algorithm>
#include <vector>
#include <memory>
#include <iterator>

#include "../src/Article.h"
#include "../src/WalkerException.h"

SUITE(ArticleTests)
{
  TEST(Article_GetNumLinks_Uninited_ThrowsException)
  {
    Article a("Foo");
    CHECK_EQUAL(a.isAnalyzed(), false);
    CHECK_THROW(a.getNumLinks(), WalkerException);
  }

  TEST(Article_GetNumLinks_Inited_ReturnsNull)
  {
    Article a("Foo");
    a.setAnalyzed(true);
    CHECK_EQUAL(a.isAnalyzed(), true);
    CHECK_EQUAL(0, a.getNumLinks());
  }

  TEST(Article_AddLinks_OneLinkAdded)
  {
    Article a("Foo");

    auto link = std::make_shared<Article>("Barmiz");
    a.addLink(link);

    CHECK_EQUAL(a.isAnalyzed(), true);
    CHECK_EQUAL(1, a.getNumLinks());
  }

  TEST(Article_AddLinks_DuplicateInstance)
  {
    Article a("Foo");

    auto arl = std::make_shared<Article>("Barmiz");
    CHECK(a.addLink(arl));
    CHECK(!a.addLink(arl));

    CHECK_EQUAL(a.isAnalyzed(), true);
    CHECK_EQUAL(1, a.getNumLinks());
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

    CHECK_EQUAL(a.isAnalyzed(), true);
    CHECK_EQUAL(3, a.getNumLinks());

    int num = 0;
    for(auto x = a.linkBegin(); x != a.linkEnd(); x++) {
      num++;
      auto atitle  = x->lock()->getTitle();
      auto findpos = std::find(titles.cbegin(), titles.cend(), atitle);
      bool isFound = (titles.end() != findpos);
      CHECK(isFound);
    }

    CHECK_EQUAL(titles.size(), num);
  }

  TEST(Article_Marked_State)
  {
    Article a("Todo");
    CHECK_EQUAL(false, a.isMarked());
    a.setMarked(true);
    CHECK_EQUAL(true, a.isMarked());
    a.setMarked(false);
    CHECK_EQUAL(false, a.isMarked());
  }
}
