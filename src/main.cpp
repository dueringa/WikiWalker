#include <iostream>
#include "WikiWalker.h"
#include "version.h"

using namespace std;

void usage(string exename);

int main(int argc, char** argv)
{
    if(argc != 2) {
        usage(argv[0]);
        return -1;
    }

    if(std::string("-v") == argv[1] || std::string("--version") == argv[1]) {
        std::cout << "WikiWalker, version " << _WW_VERSION << std::endl;
        return 0;
    }

    string url = argv[1];

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

