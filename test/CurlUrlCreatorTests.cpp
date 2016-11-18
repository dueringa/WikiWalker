#include <algorithm>

#include <UnitTest++.h>

#include "../src/CurlUrlCreator.h"

SUITE(CurlUrlCreatorTests)
{
    TEST(InitInstanceWithoutAddingParamsReturnsBaseUrl)
    {
        CurlUrlCreator cucu("http://example.com/get");
        CHECK_EQUAL("http://example.com/get", cucu.buildUrl());
    }

    TEST(AddOneParameterPair)
    {
        CurlUrlCreator cucu("http://example.com/get");
        cucu.addParameter("foo", "bar");

        auto url = cucu.buildUrl();
        CHECK(url.find("http://example.com/get?") == 0);
        CHECK_EQUAL("http://example.com/get?foo=bar", url);
    }

    TEST(AddTwoParametersSeparateCalls)
    {
        CurlUrlCreator cucu("http://example.com/get");
        cucu.addParameter("foo", "bar");
        cucu.addParameter("baz", "nul");
        auto url = cucu.buildUrl();
        // or swapped
        std::string expected = "http://example.com/get?foo=bar&baz=nul";
        CHECK_EQUAL(expected.length(),
                    url.length());
        CHECK(url.find("http://example.com/get?") == 0);
        CHECK(url.find("foo=bar") != std::string::npos);
        CHECK(url.find("baz=nul") != std::string::npos);

        CHECK_EQUAL(1, std::count(url.begin(), url.end(), '&'));
    }

    TEST(AddTwoParametersChaining)
    {
        CurlUrlCreator cucu("http://example.com/get");
        cucu.addParameter("foo", "bar")
            .addParameter("baz", "nul");
        auto url = cucu.buildUrl();
        // or swapped
        std::string expected = "http://example.com/get?foo=bar&baz=nul";
        CHECK_EQUAL(expected.length(),
                    url.length());
        CHECK(url.find("http://example.com/get?") == 0);
        CHECK(url.find("foo=bar") != std::string::npos);
        CHECK(url.find("baz=nul") != std::string::npos);
        CHECK_EQUAL(1, std::count(url.begin(), url.end(), '&'));
    }

    TEST(AddParametersAgainOverwrites)
    {
        CurlUrlCreator cucu("http://example.com/get");
        cucu.addParameter("foo", "bar");
        cucu.addParameter("baz", "nul");
        cucu.addParameter("zig", "zag");
        cucu.addParameter("baz", "kling");
        // or swapped
        std::string expected = "http://example.com/get?foo=bar&baz=kling&zig=zag";
        auto url = cucu.buildUrl();
        CHECK_EQUAL(expected.length(),
                    url.length());
        CHECK(url.find("http://example.com/get?") == 0);
        CHECK(url.find("foo=bar") != std::string::npos);
        CHECK(url.find("baz=kling") != std::string::npos);
        CHECK(url.find("zig=zag") != std::string::npos);
        CHECK_EQUAL(2, std::count(url.begin(), url.end(), '&'));
    }
}
