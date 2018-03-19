#include "GetoptCommandLineParser.h"

#include <getopt.h>

#include <iomanip>
#include <iostream>
#include <utility>

#include "CommandLineParserUtils.h"
#include "WalkerException.h"

namespace WikiWalker
{
  static struct option options[] = {
      {"help", no_argument, nullptr, 'h'},
      {"version", no_argument, nullptr, 'v'},
      {"deep", no_argument, nullptr, 'd'},
      {"dot-out", required_argument, nullptr, 'o'},
      {"json-cache", required_argument, nullptr, 'j'},
      {nullptr, 0, nullptr, 0},
  };

  GetoptCommandLineParser::~GetoptCommandLineParser() = default;

  std::string GetoptCommandLineParser::getValue(
      CommandLineParserBase::CommandLineOptions option)
  {
    return setOptions.find(CommandLineParserUtils::getStringFlag(option))
        ->second;
  }

  bool GetoptCommandLineParser::hasSet(
      CommandLineParserBase::CommandLineOptions flag)
  {
    return setOptions.find(CommandLineParserUtils::getStringFlag(flag)) !=
           setOptions.end();
  }

  void GetoptCommandLineParser::parse(int argc, char** argv)
  {
    opterr = 0;
    // ignoreing long option index
    int index = 0;

    int opt = 0;
    while((opt = getopt_long(argc, argv, "vhdj:o:", options, &index)) != -1) {
      switch(opt) {
        case 'h':
          setOptions.insert(std::make_pair("help", "1"));
          break;
        case 'v':
          setOptions.insert(std::make_pair("version", "1"));
          break;
        case 'd':
          setOptions.insert(std::make_pair("deep", "1"));
          break;
        case 'j': {
          setOptions.insert(std::make_pair("json-cache", optarg));
        } break;
        case 'o':
          setOptions.insert(std::make_pair("dot-out", optarg));
          break;
        // missing or unknows
        case '?': {
          std::string errorMsg = "Missing argument or unknown option: -";
          errorMsg.push_back(optopt);

          throw WalkerException(errorMsg);
        } break;
      }
    }

    if(optind < argc) {
      setOptions.insert(std::make_pair("url", argv[optind]));
    }
  }

  static void helpFormatter(const std::string& option,
                            const std::string& description)
  {
    std::cout << std::setw(5) << "" << std::left << std::setw(20) << option
              << "  " << description << std::endl;
  }

  void GetoptCommandLineParser::printHelp()
  {
    std::cout << "walker usage: walker [options] <URL>" << std::endl;
    std::cout << "              walker -j <file> -o <file> [URL]" << std::endl;
    std::cout << std::endl;

    helpFormatter("-h, --help", "print program help");
    helpFormatter("-v, --version", "print program version");
    helpFormatter("-d, --deep",
                  "whether to fetch and analyze linked articles as well");
    helpFormatter("-j, --json-cache", "cache file in JSON format");
    helpFormatter("-o, --dot-out", "output file for dot/graphviz");

    std::cout << std::endl;
  }
}  // namespace WikiWalker