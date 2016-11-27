//! \file BoostPoCommandLineParser.h

#ifndef _BOOSTPOCOMMANDLINEPARSER_H
#define _BOOSTPOCOMMANDLINEPARSER_H

#include "CommandLineParser.h"

#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace po = boost::program_options;

//! Command line parser using Boost::program_options
class BoostPoCommandLineParser : public CommandLineParser
{
public:
    BoostPoCommandLineParser();

    virtual std::string getValue(std::string option);
    virtual bool hasSet(std::string flag);
    virtual void parse(int argc, char** argv);
    virtual void printHelp();
private:
    po::options_description cmdOptions;
    po::options_description input;
    po::options_description cmdline_options;
    po::variables_map vm;
    po::positional_options_description p;

};

#endif // _BOOSTPOCOMMANDLINEPARSER_H
