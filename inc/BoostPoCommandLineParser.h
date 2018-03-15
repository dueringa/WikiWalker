//! \file BoostPoCommandLineParser.h

#ifndef BOOSTPOCOMMANDLINEPARSER_H
#define BOOSTPOCOMMANDLINEPARSER_H

#include <string>

#include "CommandLineParserBase.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace WikiWalker
{
  //! Command line parser using Boost::program_options
  class BoostPoCommandLineParser : public CommandLineParserBase
  {
  public:
    BoostPoCommandLineParser();

    std::string getValue(
        CommandLineParserBase::CommandLineOptions option) override;
    bool hasSet(CommandLineParserBase::CommandLineOptions flag) override;
    void parse(int argc, char** argv) override;
    void printHelp() override;

  private:
    po::options_description cmdOptions;
    po::options_description input;
    po::options_description cmdline_options;
    po::variables_map vm;
    po::positional_options_description p;
  };
}  // namespace WikiWalker
#endif  // BOOSTPOCOMMANDLINEPARSER_H
