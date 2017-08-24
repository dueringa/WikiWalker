//! \file BoostPoCommandLineParser.cpp

#include "BoostPoCommandLineParser.h"

#include <iostream>

BoostPoCommandLineParser::BoostPoCommandLineParser() :
    cmdOptions("Allowed options"),
    input("Input URLs")
{
    cmdOptions.add_options()
      ("version,v", "produce version message")
      ("help,h", "produce help message")
      ("dot-out,o", po::value<std::string>()->value_name("dotfile"), "file for dot output")
      ("json-cache,j", po::value<std::string>()->value_name("cache"), "file for json cache file")
    ;

    input.add_options()
        ("url", po::value<std::string>(), "URL")
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
    return vm.count(flag);
}

void BoostPoCommandLineParser::parse(int argc, char** argv)
{
    // po::store(po::parse_command_line(argc, argv, desc), vm);
    po::store(po::command_line_parser(argc, argv)
                  .options(cmdline_options).positional(p).run(), vm);
    po::notify(vm);
}

void BoostPoCommandLineParser::printHelp()
{
    std::cout << "Usage: walker [options] URL" << std::endl;
    std::cout << cmdOptions << std::endl;
}
