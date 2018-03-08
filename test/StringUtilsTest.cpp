#include <UnitTest++/UnitTest++.h>

#include "StringUtils.h"

SUITE(StringUtilsTest)
{
  using namespace WikiWalker;

  TEST(TestBeginString_Positives)
  {
    CHECK(StringUtils::startsWith("foo", "f"));
    CHECK(StringUtils::startsWith("Buddy", "Bud"));
    CHECK(StringUtils::startsWith("camelCase", "cam"));
  }

  TEST(TestBeginString_Equality)
  {
    CHECK(StringUtils::startsWith("foo", "foo"));
    CHECK(StringUtils::startsWith("Buddy", "Buddy"));
    CHECK(StringUtils::startsWith("camelCase", "camelCase"));
  }

  TEST(TestBeginString_SecondStringIsLonger)
  {
    CHECK(!StringUtils::startsWith("f", "foo"));
    CHECK(!StringUtils::startsWith("Bu", "Bud"));
    CHECK(!StringUtils::startsWith("cam", "camel"));
  }

  TEST(TestBeginString_EmptyString)
  {
    CHECK(!StringUtils::startsWith("foo", ""));
    CHECK(!StringUtils::startsWith("", "foo"));
    CHECK(!StringUtils::startsWith("", ""));
  }
}
