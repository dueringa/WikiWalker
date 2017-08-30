//! \file BoostPoCommandLineParser.h

#ifndef BOOSTPOCOMMANDLINEPARSER_H
#define BOOSTPOCOMMANDLINEPARSER_H

#include "CommandLineParserBase.h"

#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace po = boost::program_options;

//! Command line parser using Boost::program_options
class BoostPoCommandLineParser : public CommandLineParserBase
{
public:
  BoostPoCommandLineParser();

  virtual std::string getValue(std::string option) override;
  virtual bool hasSet(std::string flag) override;
  virtual void parse(int argc, char** argv) override;
  virtual void printHelp() override;

private:
  po::options_description cmdOptions;
  po::options_description input;
  po::options_description cmdline_options;
  po::variables_map vm;
  po::positional_options_description p;
};

#endif  // BOOSTPOCOMMANDLINEPARSER_H
