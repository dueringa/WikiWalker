#ifndef COMMANDLINEPARSERUTILS_H
#define COMMANDLINEPARSERUTILS_H

#include <string>

#include "CommandLineParserBase.h"

namespace WikiWalker
{
  //! utility functions for command line parsers
  namespace CommandLineParserUtils
  {
    //! converts command line option enum to resppective string
    std::string getStringFlag(CommandLineParserBase::CommandLineOptions flag)
    {
      std::string sflag;
      switch(flag) {
        case CommandLineParserBase::CommandLineOptions::Version:
          return "version";
        case CommandLineParserBase::CommandLineOptions::Help:
          return "help";
        case CommandLineParserBase::CommandLineOptions::URL:
          return "url";
        case CommandLineParserBase::CommandLineOptions::JsonCache:
          return "json-cache";
        case CommandLineParserBase::CommandLineOptions::DotOut:
          return "dot-out";
        case CommandLineParserBase::CommandLineOptions::FetchDeep:
          return "deep";
        default:
          return "";
      }
    }
  }  // namespace CommandLineParserUtils
}  // namespace WikiWalker
#endif  // COMMANDLINEPARSERUTILS_H