#include <iostream>
#include "WikiWalker.h"
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace po = boost::program_options;

using namespace std;

int parseOptions(int argc, char** argv, std::string& outUrl) {
    po::options_description cmdOptions("Allowed options");
    cmdOptions.add_options()
      ("version,v", "produce version message")
      ("help,h", "produce help message")
    ;

    po::options_description input("Input URLs");
    input.add_options()
        ("url", po::value<string>(), "URL")
      //("url", po::value<vector<string>>(), "URL")
    ;

    // groups
    po::options_description cmdline_options;
    cmdline_options.add(cmdOptions).add(input);

    po::positional_options_description p;
    p.add("url", 1);

    po::variables_map vm;

    try {
        // po::store(po::parse_command_line(argc, argv, desc), vm);
        po::store(po::command_line_parser(argc, argv)
                      .options(cmdline_options).positional(p).run(), vm);
        po::notify(vm);
    }
    catch(std::exception& e) {
        cerr << endl << e.what() << endl;
        cerr << endl << cmdOptions << endl;
        return -1;
    }

    if (vm.count("version")) {
        std::cout << "WikiWalker, version " << _WW_VERSION << std::endl;
        return 0;
    }

    if (vm.count("help")) {
        cout << "Usage: " << argv[0] << " [options]\n";
        cout << cmdOptions;
        return 0;
    }

    if(vm.count("url") == 0) {
        cout << "Usage: " << argv[0] << " [options]\n";
        cout << cmdOptions;
        return 1;
    }

    outUrl = vm["url"].as<string>();

    return 0;
}

void usage(string exename);

int main(int argc, char** argv)
{
    std::string url;
    int retc = parseOptions(argc, argv, url);

    if(retc || url == "")
    {
        return retc;
    }

    try {
        WikiWalker w = WikiWalker();
        w.startWalking(url);
    } catch(std::exception& e) {
        cout << "Error " << e.what() << endl;
        return -1;
    }

    return 0;
}

void usage(string exename)
{
    cout << "Usage: " << endl;
    cout << exename << " [URL]" << endl;
    cout << exename << " -v" << endl;
}

