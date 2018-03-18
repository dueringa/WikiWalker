#include <memory>
#include <sstream>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "JsonSerializer.h"

SUITE(JsonDeserializerTests)
{
  using namespace WikiWalker;

  TEST(GetArticleWithoutLinks_Unanalyzed)
  {
    std::string jsonString = R"({"Farm":{"forward_links":null}})";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);
    CHECK_EQUAL(false, a->analyzed());
  }

  TEST(GetArticleWithoutLinks_Analyzed)
  {
    std::string jsonString = R"({"Farm":{"forward_links":[]}})";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(true, a->analyzed());
    CHECK_EQUAL(0, a->countLinks());
  }

  TEST(GetArticleWithOneLink)
  {
    std::string jsonString = R"({"Farm":{"forward_links":["Animal"]}})";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    /* Does this actually make sense? Creating "nodes" that didn't exist before?
     * This is the problem I stumbled upon in JsonSerializeDeserializeTests.
     * Since Articles save linked articles as weak_ptr (18-03-17), I can't
     * silently drop them. This would lead to nullptrs.
     */

    CHECK_EQUAL(2, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(1, a->countLinks());
  }

  TEST(GetArticleWithMultipleLinks)
  {
    std::string jsonString =
        R"({"Farm":{"forward_links":["Animal","Pig","Equality"]}})";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(4, ac.countArticles());

    auto a = ac.get("Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(3, a->countLinks());
  }

  TEST(WriteEmptyArticleCollection)
  {
    std::string jsonString = "{}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);
    CHECK_EQUAL(0, ac.countArticles());
  }

  TEST(WriteArticleCollection_OneArticleWithoutLinks_Unanalyzed)
  {
    std::string jsonString = R"({"Foo":{"forward_links":null}})";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Foo");
    CHECK(a != nullptr);
    CHECK_EQUAL(false, a->analyzed());
  }

  TEST(WriteArticleCollection_OneArticleWithoutLinks_Analyzed)
  {
    std::string jsonString = R"({"Foo":{"forward_links":[]}})";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(1, ac.countArticles());

    auto a = ac.get("Foo");
    CHECK(a != nullptr);
    CHECK_EQUAL(true, a->analyzed());
    CHECK_EQUAL(0, a->countLinks());
  }
}
