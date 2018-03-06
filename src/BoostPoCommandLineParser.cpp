//! \file BoostPoCommandLineParser.cpp

#include "BoostPoCommandLineParser.h"

#include <iostream>

BoostPoCommandLineParser::BoostPoCommandLineParser()
    : cmdOptions("Allowed options"), input("Input URLs")
{
  auto& o = cmdOptions.add_options();
  o("version,v", "produce version message");
  o("help,h", "produce help message");
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

std::string BoostPoCommandLineParser::getValue(std::string option)
{
  assert(hasSet(option));
  return vm[option].as<std::string>();
}

bool BoostPoCommandLineParser::hasSet(std::string flag)
{
  return vm.count(flag) != 0u;
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
