//! \file BoostPoCommandLineParser.cpp

#include "BoostPoCommandLineParser.h"

#include <iostream>

#include "CommandLineParserUtils.h"

namespace WikiWalker
{
  BoostPoCommandLineParser::BoostPoCommandLineParser()
      : cmdOptions("Allowed options"), input("Input URLs")
  {
    auto o = cmdOptions.add_options();
    o("version,v", "produce version message");
    o("help,h", "produce help message");
    o("deep,d", "whether to fetch and analyze linked articles as well.");
    o("skip-ssl-verification", "whether to skip SSL verification");
    o("dot-out,o",
      po::value<std::string>()->value_name("dotfile"),
      "file for dot output");
    o("json-cache,j",
      po::value<std::string>()->value_name("cache"),
      "file for json cache file");

    o = input.add_options();
    o("url", po::value<std::string>(), "URL")
        //("url", po::value<vector<string>>(), "URL")
        ;

    cmdline_options.add(cmdOptions).add(input);

    p.add("url", 1);
  }

  std::string BoostPoCommandLineParser::getValue(
      CommandLineParserBase::CommandLineOptions option)
  {
    assert(hasSet(option));
    return vm[CommandLineParserUtils::getStringFlag(option)].as<std::string>();
  }

  bool BoostPoCommandLineParser::hasSet(
      CommandLineParserBase::CommandLineOptions flag)
  {
    return vm.count(CommandLineParserUtils::getStringFlag(flag)) != 0u;
  }

  void BoostPoCommandLineParser::parse(int argc, char** argv)
  {
    // po::store(po::parse_command_line(argc, argv, desc), vm);
    po::store(po::command_line_parser(argc, argv)
                  .options(cmdline_options)
                  .positional(p)
                  .run(),
              vm);
    po::notify(vm);
  }

  void BoostPoCommandLineParser::printHelp()
  {
    std::cout << "Usage: walker [options] URL" << std::endl;
    std::cout << cmdOptions << std::endl;
  }
}  // namespace WikiWalker