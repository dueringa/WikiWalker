#include <algorithm>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "WalkerException.h"
#include "WikimediaJsonToArticleConverter.h"

SUITE(WikimediaJsonToArticleConverterTests)
{
  using namespace WikiWalker;

  TEST(JsonDataWithOneLinkedArticle)
  {
    std::string testdata =
        R"({"batchcomplete":"","servedby":"mw1197","query":{"pages":[{"pageid":36669940,"ns":0,"title":"3PTT","links":[{"ns":0,"title":"Switch"}]}]}})";

    ArticleCollection ac;
    WikimediaJsonToArticleConverter conv;
    auto cont = conv.convert(testdata, ac);
    CHECK(WikimediaJsonToArticleConverter::ContinuationStatus::
              ConversionCompleted == cont);
    CHECK_EQUAL("", conv.continuationData());
    auto getArticle = ac.get("3PTT");
    CHECK(getArticle != nullptr);
    CHECK_EQUAL(1, getArticle->numLinks());
    CHECK_EQUAL(2, ac.countArticles());
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
    auto art = ac.get("FoObAr");
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
    CHECK_EQUAL("34419161|0|Jharkhand", conv.continuationData());
    auto getArticle = ac.get("Satar, Deoghar");
    CHECK(getArticle != nullptr);
    CHECK_EQUAL(1, getArticle->numLinks());
    CHECK_EQUAL(2, ac.countArticles());
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

    CHECK_EQUAL(2, ac.countAnalyzedArticles());
    auto ptr = ac.get("Zanfina Ismajli");
    CHECK(ptr != nullptr);
    ptr = ac.get("Kleite (Tochter des Danaos)");
    CHECK(ptr != nullptr);
    CHECK_EQUAL(6, ac.countArticles());
  }
}
