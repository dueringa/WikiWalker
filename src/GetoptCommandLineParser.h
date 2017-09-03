#ifndef GETOPTCOMMANDLINEPARSER_H
#define GETOPTCOMMANDLINEPARSER_H

#include <map>
#include <string>

#include "CommandLineParserBase.h"

class GetoptCommandLineParser : public CommandLineParserBase
{
public:
  virtual ~GetoptCommandLineParser();

  virtual std::string getValue(std::string option);
  virtual bool hasSet(std::string flag);
  virtual void parse(int argc, char** argv);
  virtual void printHelp();

private:
  std::map<std::string, std::string> setOptions;
};

#endif  // GETOPTCOMMANDLINEPARSER_H
