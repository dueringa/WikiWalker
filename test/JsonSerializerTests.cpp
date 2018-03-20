#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "Article.h"
#include "JsonSerializer.h"
#include "StringUtils.h"

#include "SerializerTestDefines.h"

SUITE(ArticleJsonSerializerTests)
{
  using namespace WikiWalker;

  TEST(WriteUnanalyzedArticleWithoutLinks_LinksIsNull)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;
    CollectionUtils::add(ac, std::make_shared<Article>("Farm"));

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Farm":{"forward_links":null}})") !=
          std::string::npos);
  }

  TEST(WriteAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    auto a = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, a);
    a->analyzed(true);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Farm":{"forward_links":[]}})") !=
          std::string::npos);
  }

  TEST(WriteArticleWithOneLink)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, a);

    auto linked = std::make_shared<Article>("Animal");
    a->addLink(linked);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Farm":{"forward_links":["Animal"]}})") !=
          std::string::npos);
  }

  TEST(WriteArticleWithMultipleLinks)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, a);

    auto al1 = std::make_shared<Article>("Animal"),
         al2 = std::make_shared<Article>("Pig"),
         al3 = std::make_shared<Article>("Equality");

    a->addLink(al1);
    a->addLink(al2);
    a->addLink(al3);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(
              WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
              R"({"Farm":{"forward_links":["Animal","Pig","Equality"]}})") !=
          std::string::npos);
  }

  TEST(WriteEmptyArticleCollection)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY "{}") !=
          std::string::npos);
  }

  TEST(WriteArticleCollection_OneUnanalyzedArticleWithoutLinks_LinksIsNull)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto linked = std::make_shared<Article>("Foo");
    CollectionUtils::add(ac, linked);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Foo":{"forward_links":null}})") !=
          std::string::npos);
  }

  TEST(WriteArticleCollection_OneAnalyzedArticleWithoutLinks_LinksIsEmptyArray)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto a = std::make_shared<Article>("Foo");
    a->analyzed(true);
    CollectionUtils::add(ac, a);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Foo":{"forward_links":[]}})") !=
          std::string::npos);
  }

  TEST(
      WriteArticleCollection_MultipleArticles_WithMultipleLinks_MatchesExpected)
  {
    JsonSerializer atj;
    ArticleCollection ac;
    std::ostringstream oss;

    auto a = std::make_shared<Article>("Foo");
    auto b = std::make_shared<Article>("Bar");
    auto c = std::make_shared<Article>("Baz");
    a->addLink(b);
    b->addLink(a);
    b->addLink(c);
    CollectionUtils::add(ac, a);
    CollectionUtils::add(ac, b);
    CollectionUtils::add(ac, c);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         "{"
                         R"("Bar":{"forward_links":["Foo","Baz"]},)"
                         R"("Baz":{"forward_links":null},)"
                         R"("Foo":{"forward_links":["Bar"]})"
                         "}") != std::string::npos);
  }

  TEST(SerializeArticleWithOnlyNullptr_NullptrWillBeSkipped)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, a);

    {
      // will be skipped on serialization, since it'll become nullptr
      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
    }

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Farm":{"forward_links":[]}})") !=
          std::string::npos);
  }

  TEST(SerializeArticleWithValidArticleAndANullptr_NullptrWillBeSkipped)
  {
    JsonSerializer atj;
    std::ostringstream oss;
    ArticleCollection ac;

    // yes, only a is inserted, since we want to emulate article-only
    auto a = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, a);

    {
      // will be skipped on serialization, since it'll become nullptr
      auto linked = std::make_shared<Article>("Animal");
      a->addLink(linked);
    }

    auto linked2 = std::make_shared<Article>("Barn");
    a->addLink(linked2);

    atj.serialize(ac, oss);

    auto serString = oss.str();
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_1) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_HEADER_2) !=
          std::string::npos);
    CHECK(serString.find(WW_TEST_JSONSERIALIZER_PROTOCOL_COLLECTION_KEY
                         R"({"Farm":{"forward_links":["Barn"]}})") !=
          std::string::npos);
  }
}
