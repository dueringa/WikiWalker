#ifndef GETOPTCOMMANDLINEPARSER_H
#define GETOPTCOMMANDLINEPARSER_H

#include <map>
#include <string>

#include "CommandLineParserBase.h"

class GetoptCommandLineParser : public CommandLineParserBase
{
public:
  virtual ~GetoptCommandLineParser();

  std::string getValue(std::string option) override;
  bool hasSet(std::string flag) override;
  void parse(int argc, char** argv) override;
  void printHelp() override;

private:
  std::map<std::string, std::string> setOptions;
};

#endif  // GETOPTCOMMANDLINEPARSER_H
