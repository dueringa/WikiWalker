//! \file main.cpp

#include <iostream>
#include <fstream>

#include "WikiWalker.h"
#include "version.h"
#include "BoostPoCommandLineParser.h"
#include "ToGraphvizWriter.h"

using namespace std;

int main(int argc, char** argv)
{
    BoostPoCommandLineParser cmdp;

    try {
        cmdp.parse(argc, argv);
    } catch(std::exception& e) {
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
    bool isDotSet = cmdp.hasSet("dot-out");


    bool read_failed = false;
    std::string url = cmdp.getValue("url");
    WikiWalker w;

    if(cmdp.hasSet("json-cache")) {
        try {
            std::string cachefile = cmdp.getValue("json-cache");
            w.readCache(cachefile);
        } catch(std::exception& e) {
            std::cout << e.what() << endl;
            read_failed = true;
        }
    }

    try {
        w.startWalking(url);
    } catch(std::exception& e) {
        cout << "Error " << e.what() << endl;
        return -1;
    }

    if(cmdp.hasSet("json-cache")) {
        if(read_failed) {
            cout << "Reading from cache failed, won't overwrite" << endl;
        } else {
            try {
                std::string cachefile = cmdp.getValue("json-cache");
                w.writeCache(cachefile);
            } catch(std::exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    }

    if(isDotSet) {
        const ArticleCollection& ac = w.getCollection();
        std::string outfile = cmdp.getValue("dot-out");
        ToGraphvizWriter tgw;
        ofstream file(outfile, ios::trunc | ios::out);

        if(file.fail()) {
            cerr << "Error opening dot out file for writing" << endl;
        } else {
            tgw.output(ac, file);
            file.flush();

            if(file.bad() || file.fail()) {
                cerr << "Error during writing dot out file." << endl;
            }

            file.close();
        }
    }
    return 0;
}
