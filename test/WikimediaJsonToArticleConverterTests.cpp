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
    auto cont = conv.convertToArticle(testdata, ac);
    CHECK(WikimediaJsonToArticleConverter::ContinuationStatus::
              ConversionCompleted == cont);
    CHECK_EQUAL("", conv.getContinuationData());
    auto getArticle = ac.get("3PTT");
    CHECK(getArticle != nullptr);
    CHECK_EQUAL(1, getArticle->getNumLinks());
    CHECK_EQUAL(2, ac.getNumArticles());
  }

  TEST(JsonDataWithInvalidArticle_Throws)
  {
    std::string testdata =
        R"({"batchcomplete":"","servedby":"mw1208","query":{"pages":[{"ns":0,"title":"FoObAr","missing":""}]}})";

    ArticleCollection ac;
    WikimediaJsonToArticleConverter conv;
    CHECK_THROW(conv.convertToArticle(testdata, ac), WalkerException);
  }

  TEST(JsonData_MoreLinks_HasContinueData)
  {
    std::string testdata =
        R"({"continue":{"plcontinue":"34419161|0|Jharkhand","continue":"||"},"servedby":"mw1283","query":{"pages":[{"pageid":34419161,"ns":0,"title":"Satar, Deoghar","links":[{"ns":0,"title":"Deoghar district"}]}]}})";

    ArticleCollection ac;
    WikimediaJsonToArticleConverter conv;
    auto cont = conv.convertToArticle(testdata, ac);
    CHECK(cont == WikimediaJsonToArticleConverter::ContinuationStatus::
                      ConversionNeedsMoreData);
    CHECK_EQUAL("34419161|0|Jharkhand", conv.getContinuationData());
    auto getArticle = ac.get("Satar, Deoghar");
    CHECK(getArticle != nullptr);
    CHECK_EQUAL(1, getArticle->getNumLinks());
    CHECK_EQUAL(2, ac.getNumArticles());
  }
}
