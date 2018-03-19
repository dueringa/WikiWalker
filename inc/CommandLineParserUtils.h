#ifndef WIKIWALKER_COMMANDLINEPARSERUTILS_H
#define WIKIWALKER_COMMANDLINEPARSERUTILS_H

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
        case CommandLineParserBase::CommandLineOptions::SkipSslVerification:
          return "skip-ssl-verification";
        default:
          return "";
      }
    }
  }  // namespace CommandLineParserUtils
}  // namespace WikiWalker
#endif  // WIKIWALKER_COMMANDLINEPARSERUTILS_H
