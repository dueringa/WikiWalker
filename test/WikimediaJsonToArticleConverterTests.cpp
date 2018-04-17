#include <algorithm>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "WalkerException.h"
#include "WikimediaJsonToArticleConverter.h"

SUITE(WikimediaJsonToArticleConverterTests)
{
  using namespace WikiWalker;
  using namespace WikiWalker::CollectionUtils;

  TEST(JsonDataWithOneLinkedArticle)
  {
    std::string testdata =
        R"({"batchcomplete":"","servedby":"mw1197","query":{"pages":[{"pageid":36669940,"ns":0,"title":"3PTT","links":[{"ns":0,"title":"Switch"}]}]}})";

    ArticleCollection ac;
    WikimediaJsonToArticleConverter conv;
    auto cont = conv.convert(testdata, ac);
    CHECK(WikimediaJsonToArticleConverter::ContinuationStatus::
              ConversionCompleted == cont);
    CHECK_EQUAL("", conv.continuationData()["plcontinue"]);
    auto getArticle = CollectionUtils::get(ac, "3PTT");
    CHECK(getArticle != nullptr);
    CHECK_EQUAL(1, getArticle->countLinks());
    CHECK_EQUAL(2, ac.size());
  }

  TEST(JsonDataWithInvalidArticle_Throws)
  {
    std::string testdata =
        R"({"batchcomplete":"","servedby":"mw1208","query":{"pages":[{"ns":0,"title":"FoObAr","missing":""}]}})";

    ArticleCollection ac;
    WikimediaJsonToArticleConverter conv;
    auto ret = conv.convert(testdata, ac);
    CHECK(ret == WikiWalker::WikimediaJsonToArticleConverter::
                     ContinuationStatus::ConversionCompleted);
    auto art = CollectionUtils::get(ac, "FoObAr");
    CHECK(art != nullptr);
    CHECK(art->marked());
  }

  TEST(JsonData_MoreLinks_HasContinueData)
  {
    std::string testdata =
        R"({"continue":{"plcontinue":"34419161|0|Jharkhand","continue":"||"},"servedby":"mw1283","query":{"pages":[{"pageid":34419161,"ns":0,"title":"Satar, Deoghar","links":[{"ns":0,"title":"Deoghar district"}]}]}})";

    ArticleCollection ac;
    WikimediaJsonToArticleConverter conv;
    auto cont = conv.convert(testdata, ac);
    CHECK(cont == WikimediaJsonToArticleConverter::ContinuationStatus::
                      ConversionNeedsMoreData);
    CHECK_EQUAL("34419161|0|Jharkhand", conv.continuationData()["plcontinue"]);
    auto getArticle = CollectionUtils::get(ac, "Satar, Deoghar");
    CHECK(getArticle != nullptr);
    CHECK_EQUAL(1, getArticle->countLinks());
    CHECK_EQUAL(2, ac.size());
  }

  TEST(JsonData_ContainsMultipleArticles)
  {
    std::string testdata =
        R"#({"batchcomplete": true,"query": {"normalized": [{"fromencoded": false,"from": "Zanfina_Ismajli","to": "Zanfina Ismajli"},{"fromencoded": false,"from": "Kleite_(Tochter_des_Danaos)","to": "Kleite (Tochter des Danaos)"}],"pages": [{"pageid": 2834303,"ns": 0,"title": "Zanfina Ismajli","links": [{"ns": 0,"title": "10. Mai"},{"ns": 0,"title": "1985"}]},{"pageid": 8086803,"ns": 0,"title": "Kleite (Tochter des Danaos)","links": [{"ns": 0,"title": "Aigyptos"},{"ns": 0,"title": "Altgriechische Sprache"}]}]},"limits": {"links": 500}})#";
    WikimediaJsonToArticleConverter conv;
    ArticleCollection ac;
    auto cont = conv.convert(testdata, ac);
    CHECK(WikimediaJsonToArticleConverter::ContinuationStatus::
              ConversionCompleted == cont);

    CHECK_EQUAL(2, CollectionUtils::countAnalyzedArticles(ac));
    auto ptr = CollectionUtils::get(ac, "Zanfina Ismajli");
    CHECK(ptr != nullptr);
    ptr = CollectionUtils::get(ac, "Kleite (Tochter des Danaos)");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(6, ac.size());
  }

  TEST(JsonData_ConvertLinkshere)
  {
    std::string testdata =
        R"#({"batchcomplete":true,"query":{"pages":[{"pageid":2,"ns":0,"title":"Eins","linkshere":[{"ns":0,"title":"Zw\u00f6lf"},{"ns":0,"title":"Dreizehn"},{"ns":0,"title":"Ens"}]}]},"limits":{"linkshere":500}})#";
    WikimediaJsonToArticleConverter conv;
    ArticleCollection ac;
    auto cont = conv.convert(testdata, ac);

    // exp: zwölf, dreizehn, ens --> eins
    CHECK(WikimediaJsonToArticleConverter::ContinuationStatus::
              ConversionCompleted == cont);
    CHECK_EQUAL(3, CollectionUtils::countAnalyzedArticles(ac));
    CHECK_EQUAL(4, ac.size());

    auto ptr = CollectionUtils::get(ac, "Ens");
    REQUIRE CHECK(ptr != nullptr);
    CHECK(ptr->analyzed());
    ptr = CollectionUtils::get(ac, "Dreizehn");
    REQUIRE CHECK(ptr != nullptr);
    CHECK(ptr->analyzed());
    // better be save than sorry
    ptr = CollectionUtils::get(ac, "Zw\u00f6lf");
    REQUIRE CHECK(ptr != nullptr);
    CHECK(ptr->analyzed());
    ptr = CollectionUtils::get(ac, "Eins");
    REQUIRE CHECK(ptr != nullptr);
    CHECK(!ptr->analyzed());
  }
}
