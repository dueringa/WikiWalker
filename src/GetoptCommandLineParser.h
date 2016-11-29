#ifndef _GETOPTCOMMANDLINEPARSER_H
#define _GETOPTCOMMANDLINEPARSER_H

#include "CommandLineParser.h"

#include <map>

class GetoptCommandLineParser : public CommandLineParser
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

#endif // _GETOPTCOMMANDLINEPARSER_H