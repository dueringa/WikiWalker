#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

#include "Article.h"
#include "ArticleCollection.h"
#include "CacheJsonToArticleConverter.h"
#include "ToGraphvizWriter.h"

int main(int argc, char** argv)
{
  if(argc != 3) {
    std::cout << argv[0] << " "
              << "<cache file>"
              << "<dot file>" << std::endl;
    return 1;
  }

  WikiWalker::CacheJsonToArticleConverter cjta;
  std::ifstream cache(argv[1]);

  // assumption: having write-only access to a file is so rare that I don't care
  // also, currently the file is used for both read and write, so initially it
  // won't exist.
  if(!cache.is_open()) {
    std::cerr << "Couldn't open cache file" << std::endl;
    return 1;
  }

  WikiWalker::ArticleCollection ac;
  cjta.convertToArticle(cache, ac);

  if(cache.fail()) {
    cache.close();
    std::cerr << "Error reading from file" << std::endl;
    return 1;
  }

  WikiWalker::ArticleCollection filteredColl;
  for(auto& anArticle : ac) {
    auto art = anArticle.second;
    if(art->isAnalyzed()) {
      filteredColl.add(art);
    }
  }

  WikiWalker::ToGraphvizWriter g;
  std::ofstream filteredGraph(argv[2]);

  if(!filteredGraph.is_open()) {
    std::cerr << "Couldn't open dot file for writing" << std::endl;
    return 1;
  }

  g.output(filteredColl, filteredGraph);

  if(filteredGraph.fail()) {
    filteredGraph.close();
    std::cerr << "Error writing to file" << std::endl;
    return 1;
  }

  filteredGraph.close();

  std::cout << "There are " << filteredColl.getNumArticles()
            << " analyzed articles" << std::endl;

  return 0;
}
