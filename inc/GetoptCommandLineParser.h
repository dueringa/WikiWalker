#ifndef WIKIWALKER_GETOPTCOMMANDLINEPARSER_H
#define WIKIWALKER_GETOPTCOMMANDLINEPARSER_H

#include <map>
#include <string>

#include "CommandLineParserBase.h"

namespace WikiWalker
{
  //! command line parser using getopt functions
  class GetoptCommandLineParser : public CommandLineParserBase
  {
  public:
    ~GetoptCommandLineParser() override = default;

    std::string getValue(
        CommandLineParserBase::CommandLineOptions option) override;
    bool hasSet(CommandLineParserBase::CommandLineOptions flag) override;
    void parse(int argc, char** argv) override;
    void printHelp() override;

  private:
    std::map<std::string, std::string> setOptions;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_GETOPTCOMMANDLINEPARSER_H
