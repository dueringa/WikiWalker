#include <UnitTest++.h>

#include "../src/WikimediaJsonToArticleConverter.h"
#include "../src/Article.h"
#include "../src/WalkerException.h"

SUITE(JsonToArticleConverterTests)
{
    TEST(JsonDataWithOneLinkedArticle)
    {
        std::string testdata = "{\"batchcomplete\":\"\",\"servedby\":\"mw1197\",\"query\":{\"pages\":{\"36669940\":{\"pageid\":36669940,\"ns\":0,\"title\":\"3PTT\",\"links\":[{\"ns\":0,\"title\":\"Switch\"}]}}}}";

        ArticleCollection ac;
        WikimediaJsonToArticleConverter conv;
        auto art = conv.convertToArticle(testdata, ac);
        CHECK(art != nullptr);
        CHECK(!conv.hasMoreData());
        CHECK_EQUAL("", conv.getContinuationData());
        CHECK_EQUAL(1, art->getNumLinks());
        CHECK_EQUAL(2, ac.getNumArticles());
    }

    TEST(JsonDataWithInvalidArticle_Throws)
    {
        std::string testdata = "{\"batchcomplete\":\"\",\"servedby\":\"mw1208\",\"query\":{\"pages\":{\"-1\":{\"ns\":0,\"title\":\"FoObAr\",\"missing\":\"\"}}}}";

        ArticleCollection ac;
        WikimediaJsonToArticleConverter conv;
        CHECK_THROW(conv.convertToArticle(testdata, ac), WalkerException);
    }

    TEST(JsonData_MoreLinks_HasContinueData)
    {
        std::string testdata = "{\"continue\":{\"plcontinue\":\"34419161|0|Jharkhand\",\"continue\":\"||\"},\"servedby\":\"mw1283\",\"query\":{\"pages\":{\"34419161\":{\"pageid\":34419161,\"ns\":0,\"title\":\"Satar, Deoghar\",\"links\":[{\"ns\":0,\"title\":\"Deoghar district\"}]}}}}";

        ArticleCollection ac;
        WikimediaJsonToArticleConverter conv;
        auto art = conv.convertToArticle(testdata, ac);
        CHECK(art != nullptr);
        CHECK(conv.hasMoreData());
        CHECK_EQUAL("34419161|0|Jharkhand", conv.getContinuationData());
        CHECK_EQUAL(1, art->getNumLinks());
        CHECK_EQUAL(2, ac.getNumArticles());
    }
}
