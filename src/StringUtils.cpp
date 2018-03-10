//! \file StringUtils.cpp

#include "StringUtils.h"

namespace WikiWalker
{
  bool StringUtils::startsWith(std::string stringToTest, std::string beginning)
  {
    if(stringToTest.empty() || beginning.empty()) {
      return false;
    }

    // can't be starting with beginning if it's shorter
    if(stringToTest.length() < beginning.length()) {
      return false;
    }

    // suffice to compare n characters, where n is length of beginning
    if(stringToTest.substr(0, beginning.length()).compare(beginning) == 0) {
      return true;
    }

    return false;
  }
}