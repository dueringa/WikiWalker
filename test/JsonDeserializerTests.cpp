#include <memory>
#include <sstream>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "JsonSerializer.h"
#include "WalkerException.h"

#include "SerializerTestDefines.h"

#define WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE \
  "{" WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1   \
  "," WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2   \
  "," WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY

SUITE(JsonDeserializerTests)
{
  using namespace WikiWalker;

  TEST(Deserialize_ArticleCollection_OneArticleWithoutLinks_Unanalyzed)
  {
    std::string jsonString = WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE
        R"({"Farm":{"forward_links":null}})"
        "}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(1, ac.size());

    auto a = CollectionUtils::get(ac, "Farm");
    CHECK(a != nullptr);
    CHECK_EQUAL(false, a->analyzed());
  }

  TEST(Deserialize_ArticleCollection_OneArticleWithoutLinks_Analyzed)
  {
    std::string jsonString = WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE
        R"({"Farm":{"forward_links":[]}})"
        "}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    CHECK_EQUAL(1, ac.size());

    auto a = CollectionUtils::get(ac, "Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(true, a->analyzed());
    CHECK_EQUAL(0, a->countLinks());
  }

  TEST(Deserialize_ArticleCollection_OneArticleWithOneLink)
  {
    std::string jsonString = WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE
        R"({"Farm":{"forward_links":["Animal"]}})"
        "}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    /* Deserialized state doesn't exactly match serialized one.
     * This is intended. Only-link articles become new articles in the
     * collection.
     */
    CHECK_EQUAL(2, ac.size());

    auto a = CollectionUtils::get(ac, "Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(1, a->countLinks());
  }

  TEST(Deserialize_ArticleCollection_OneArticleWithMultipleLinks)
  {
    std::string jsonString = WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE
        R"({"Farm":{"forward_links":["Animal","Pig","Equality"]}})"
        "}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    // see comment in Deserialize_ArticleCollection_OneArticleWithOneLink
    CHECK_EQUAL(4, ac.size());

    auto a = CollectionUtils::get(ac, "Farm");
    CHECK(a != nullptr);

    CHECK_EQUAL(3, a->countLinks());
  }

  TEST(Deserialize_ArticleCollection_Empty)
  {
    std::string jsonString = WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE
        "{}"
        "}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);
    CHECK_EQUAL(0, ac.size());
  }

  TEST(Deserialize_ArticleCollection_MultipleArticlesWithMultipleLinks)
  {
    std::string jsonString = WW_TEST_JSONSERIALIZER_PROTOCOL_COMPLETE
        R"({)"
        R"("Farm":)"
        R"({"forward_links":["Animal","Pig","Equality"]},)"
        R"("Animal":)"
        R"({"forward_links":["Cat","Pig","Dog"]})"
        R"(})"
        "}";
    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    deser.deserialize(ac, json);

    // see comment in Deserialize_ArticleCollection_OneArticleWithOneLink
    CHECK_EQUAL(6, ac.size());

    for(std::string a : {"Farm", "Animal", "Pig", "Equality", "Cat", "Dog"}) {
      auto x = CollectionUtils::get(ac, a);
      CHECK(x != nullptr);
    }

    auto a = CollectionUtils::get(ac, "Farm");
    CHECK(a != nullptr);
    CHECK_EQUAL(3, a->countLinks());
  }

  TEST(Deserialize_ArticleCollection_WrongProgramName)
  {
    std::string jsonString =
        R"({"program":"wikwalker","scheme-version":2,)"
        R"("ArticleCollection":)"
        R"({"Farm":{"forward_links":null}})"
        "}";

    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    CHECK_THROW(deser.deserialize(ac, json), WalkerException);
  }

  TEST(Deserialize_ArticleCollection_WrongVersion)
  {
    std::string jsonString =
        R"({"program":"wikiwalker","scheme-version":3,)"
        R"("ArticleCollection":)"
        R"({"Farm":{"forward_links":null}})"
        "}";

    std::istringstream json(jsonString);

    ArticleCollection ac;
    JsonSerializer deser;
    CHECK_THROW(deser.deserialize(ac, json), WalkerException);
  }
}
