//! \file main.cpp

#include <fstream>
#include <iostream>
#include <string>

#include "ToGraphvizWriter.h"
#include "WikiWalker.h"
#include "version.h"

#include "config.h"

int main(int argc, char** argv)
{
#if defined(WW_USE_BOOST_PO)
  WikiWalker::BoostPoCommandLineParser cmdp;
#elif defined(WW_USE_GETOPT)
  WikiWalker::GetoptCommandLineParser cmdp;
#endif

  try {
    cmdp.parse(argc, argv);
  } catch(std::exception& e) {
    std::cerr << std::endl << e.what() << std::endl;
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
    std::cerr << "Must either specify at least URL, "
              << "or dot and cache file." << std::endl;
    cmdp.printHelp();
    return 1;
  }

  bool read_failed = false;
  WikiWalker::WikiWalker w;

  if(isCacheSet) {
    try {
      std::string cachefile = cmdp.getValue("json-cache");
      w.readCache(cachefile);
    } catch(std::exception& e) {
      std::cout << e.what() << std::endl;
      read_failed = true;
    }
  }

  if(isUrlSet) {
    try {
      std::string url = cmdp.getValue("url");
      w.startWalking(url);
    } catch(std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      return -1;
    }
  }

  if(isCacheSet) {
    std::string cachefile = cmdp.getValue("json-cache");
    if(read_failed) {
      cachefile.append("_");
      std::cout << "Reading from cache failed, write to " << cachefile
                << std::endl;
    }
    try {
      w.writeCache(cachefile);
    } catch(std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
    }
  }

  if(isDotSet) {
    const WikiWalker::ArticleCollection& ac = w.getCollection();
    std::string outfile                     = cmdp.getValue("dot-out");
    WikiWalker::ToGraphvizWriter tgw;
    std::ofstream file(outfile, std::ios::trunc | std::ios::out);

    if(file.fail()) {
      std::cerr << "Error opening dot out file for writing" << std::endl;
    } else {
      tgw.output(ac, file);
      file.flush();

      if(file.bad() || file.fail()) {
        std::cerr << "Error during writing dot out file." << std::endl;
      }

      file.close();
    }
  }

  return 0;
}
