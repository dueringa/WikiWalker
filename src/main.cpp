//! \file main.cpp

#include <fstream>
#include <iostream>
#include <string>

#include "Article.h"
#include "ToGraphvizWriter.h"
#include "WikiWalker.h"
#include "version.h"

#include "config.h"

using CmdOpt = WikiWalker::CommandLineParserBase::CommandLineOptions;

//! limit for printing article links / contents
const int printLimit = 10;

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

  if(cmdp.hasSet(CmdOpt::Version)) {
    std::cout << "WikiWalker, version " << _WW_VERSION << std::endl;
    return 0;
  }

  if(cmdp.hasSet(CmdOpt::Help)) {
    cmdp.printHelp();
    return 0;
  }

  bool isUrlSet       = cmdp.hasSet(CmdOpt::URL);
  bool isCacheSet     = cmdp.hasSet(CmdOpt::JsonCache);
  bool isDotSet       = cmdp.hasSet(CmdOpt::DotOut);
  bool isDeepSet      = cmdp.hasSet(CmdOpt::FetchDeep);
  bool validRunConfig = isUrlSet || (isDotSet && isCacheSet);

  if(!validRunConfig) {
    std::cerr << "Must either specify at least URL, "
              << "or dot and cache file." << std::endl;
    cmdp.printHelp();
    return 1;
  }

  if(isUrlSet && isDeepSet && !isCacheSet) {
    std::cerr << "Please specify a cache file when using \"deep\" option"
              << std::endl;
    cmdp.printHelp();
    return 1;
  }

  bool read_failed = false;
  WikiWalker::WikiWalker w;

  if(isCacheSet) {
    try {
      std::string cachefile = cmdp.getValue(CmdOpt::JsonCache);
      w.readCache(cachefile);
    } catch(std::exception& e) {
      std::cout << e.what() << std::endl;
      read_failed = true;
    }
  }

  if(isUrlSet) {
    try {
      std::string url = cmdp.getValue(CmdOpt::URL);
      w.startWalking(url);
    } catch(std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      return -1;
    }
  }

  if(isCacheSet) {
    std::string cachefile = cmdp.getValue(CmdOpt::JsonCache);
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
    std::string outfile                     = cmdp.getValue(CmdOpt::DotOut);
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

  size_t numArt = w.getCollection().getNumAnalyzedArticles();
  if(numArt > 10) {
    std::cout << "There are " << numArt << " analyzed articles."
              << " Not printing them. (Limit: " << printLimit << ")."
              << std::endl;
  } else {
    for(auto& a : w.getCollection()) {
      auto& art = a.second;
      if(art->isAnalyzed()) {
        std::cout << "Article " << a.first << " has " << art->getNumLinks()
                  << " links" << std::endl;
      }
    }
  }

  return 0;
}
