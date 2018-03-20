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
    std::string getStringFlag(CommandLineParserBase::CommandLineOptions flag);
  }  // namespace CommandLineParserUtils
}  // namespace WikiWalker
#endif  // WIKIWALKER_COMMANDLINEPARSERUTILS_H
