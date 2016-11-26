//! \file main.cpp

#include <iostream>

#include "WikiWalker.h"
#include "version.h"
#include "BoostPoCommandLineParser.h"

using namespace std;

int main(int argc, char** argv)
{
    BoostPoCommandLineParser cmdp;

    try {
        cmdp.parse(argc, argv);
    }
    catch(std::exception& e) {
        cerr << endl << e.what() << endl;
        cmdp.printHelp();
        return -1;
    }

    if (cmdp.hasSet("version")) {
        std::cout << "WikiWalker, version " << _WW_VERSION << std::endl;
        return 0;
    }

    if (cmdp.hasSet("help")) {
        cmdp.printHelp();
        return 0;
    }

    if(!cmdp.hasSet("url")) {
        cerr << "Missing URL" << endl;
        cmdp.printHelp();
        return 1;
    }

    std::string url = cmdp.getValue("url");
    WikiWalker w;

    try {
        w.startWalking(url);
    } catch(std::exception& e) {
        cout << "Error " << e.what() << endl;
        return -1;
    }

    try {
        if(cmdp.hasSet("json-cache")) {
            std::string cachefile = cmdp.getValue("json-cache");
            w.writeCache(cachefile);
        }
    }
    catch(std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }



    return 0;
}
