//! \file main.cpp

#include <iostream>

#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "WikiWalker.h"
#include "version.h"

namespace po = boost::program_options;

using namespace std;

int main(int argc, char** argv)
{
    po::options_description cmdOptions("Allowed options");
    cmdOptions.add_options()
      ("version,v", "produce version message")
      ("help,h", "produce help message")
      ("dot-out,o", po::value<string>(), "file for dot output (unused)")
      ("json-cache,j", po::value<string>(), "file for json cache file (unused)")
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

    if(!vm.count("url")) {
        cout << "Usage: " << argv[0] << " [options]\n";
        cout << cmdOptions;
        return 1;
    }

    std::string url = vm["url"].as<string>();

    try {
        WikiWalker w;
        w.startWalking(url);
    } catch(std::exception& e) {
        cout << "Error " << e.what() << endl;
        return -1;
    }

    return 0;
}
