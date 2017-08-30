//! \file main.cpp

#include <iostream>
#include <fstream>

#include "WikiWalker.h"
#include "ToGraphvizWriter.h"
#include "version.h"

#include "config.h"

using namespace std;

int main(int argc, char** argv)
{
#if defined(WW_USE_BOOST_PO)
  BoostPoCommandLineParser cmdp;
#elif defined(WW_USE_GETOPT)
  GetoptCommandLineParser cmdp;
#endif

  try {
    cmdp.parse(argc, argv);
  } catch(std::exception& e) {
    cerr << endl << e.what() << endl;
    cmdp.printHelp();
    return -1;
  }

  if(cmdp.hasSet("version")) {
    std::cout << "WikiWalker, version " << _WW_VERSION << std::endl;
    return 0;
  }

  if(cmdp.hasSet("help")) {
    cmdp.printHelp();
    return 0;
  }

  bool isUrlSet       = cmdp.hasSet("url");
  bool isCacheSet     = cmdp.hasSet("json-cache");
  bool isDotSet       = cmdp.hasSet("dot-out");
  bool validRunConfig = isUrlSet || (isDotSet && isCacheSet);

  if(!validRunConfig) {
    cerr << "Must either specify at least URL, "
         << "or dot and cache file." << endl;
    cmdp.printHelp();
    return 1;
  }

  bool read_failed = false;
  WikiWalker w;

  if(isCacheSet) {
    try {
      std::string cachefile = cmdp.getValue("json-cache");
      w.readCache(cachefile);
    } catch(std::exception& e) {
      std::cout << e.what() << endl;
      read_failed = true;
    }
  }

  if(isUrlSet) {
    try {
      std::string url = cmdp.getValue("url");
      w.startWalking(url);
    } catch(std::exception& e) {
      cout << "Error " << e.what() << endl;
      return -1;
    }
  }

  if(isCacheSet) {
    std::string cachefile = cmdp.getValue("json-cache");
    if(read_failed) {
      cachefile.append("_");
      cout << "Reading from cache failed, write to " << cachefile << endl;
    }
    try {
      w.writeCache(cachefile);
    } catch(std::exception& e) {
      cout << "Error: " << e.what() << endl;
    }
  }

  if(isDotSet) {
    const ArticleCollection& ac = w.getCollection();
    std::string outfile         = cmdp.getValue("dot-out");
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
