#include <fstream>
#include <memory>

#include <UnitTest++/UnitTest++.h>

#include "Article.h"
#include "ToGraphvizWriter.h"

// these test always pass, unless exceptions occur
//! TODO: try to initialize some variables only in one place

SUITE(ArticleToGraphvizWriterTests)
{
  using namespace WikiWalker;
  using namespace WikiWalker::CollectionUtils;

  TEST(WriteArticleWithMultipleLinks)
  {
    std::ofstream outfile("_artlinks.dot",
                          std::ofstream::out | std::ofstream::trunc);
    ToGraphvizWriter atj;
    Article a("Farm");

    auto al1 = std::make_shared<Article>("Animal"),
         al2 = std::make_shared<Article>("Pig"),
         al3 = std::make_shared<Article>("Equality");

    a.addLink(al1);
    a.addLink(al2);
    a.addLink(al3);
    atj.output(&a, outfile);
    outfile.flush();
    outfile.close();
  }

  TEST(WriteArticleCollectionDepthOne)
  {
    std::ofstream outfile("_artctree.dot",
                          std::ofstream::out | std::ofstream::trunc);
    ToGraphvizWriter atj;
    ArticleCollection ac;

    auto farm = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, farm);

    auto animal = std::make_shared<Article>("Animal");
    farm->addLink(animal);
    CollectionUtils::add(ac, animal);

    auto pig = std::make_shared<Article>("Pig");
    farm->addLink(pig);
    CollectionUtils::add(ac, pig);

    auto equa = std::make_shared<Article>("Equality");
    farm->addLink(equa);
    CollectionUtils::add(ac, equa);

    atj.output(ac, outfile);
    outfile.flush();
    outfile.close();
  }

  TEST(WriteArticleCollectionDepthOne_WithInterlinks)
  {
    std::ofstream outfile("_artctree_interlink.dot",
                          std::ofstream::out | std::ofstream::trunc);
    ToGraphvizWriter atj;
    ArticleCollection ac;

    auto farm = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, farm);

    auto animal = std::make_shared<Article>("Animal");
    farm->addLink(animal);
    CollectionUtils::add(ac, animal);

    auto pig = std::make_shared<Article>("Pig");
    pig->marked(true);
    farm->addLink(pig);
    CollectionUtils::add(ac, pig);

    pig->addLink(animal);
    animal->addLink(pig);

    auto equa = std::make_shared<Article>("Equality");
    farm->addLink(equa);
    CollectionUtils::add(ac, equa);

    atj.output(ac, outfile);
    outfile.flush();
    outfile.close();
  }

  TEST(WriteArticleCollectionDepthTwo)
  {
    std::ofstream outfile("_artc_deeptree.dot",
                          std::ofstream::out | std::ofstream::trunc);
    ToGraphvizWriter atj;
    ArticleCollection ac;

    auto farm = std::make_shared<Article>("Farm");
    CollectionUtils::add(ac, farm);

    auto animal = std::make_shared<Article>("Animal");
    farm->addLink(animal);
    CollectionUtils::add(ac, animal);

    auto pig = std::make_shared<Article>("Pig");
    farm->addLink(pig);
    CollectionUtils::add(ac, pig);

    auto equa = std::make_shared<Article>("Equality");
    farm->addLink(equa);
    CollectionUtils::add(ac, equa);

    auto rights = std::make_shared<Article>("Rights");
    equa->addLink(rights);
    CollectionUtils::add(ac, rights);

    auto cat = std::make_shared<Article>("Cat");
    CollectionUtils::add(ac, cat);
    animal->addLink(cat);
    auto dog = std::make_shared<Article>("Dog");
    CollectionUtils::add(ac, dog);
    animal->addLink(dog);

    atj.output(ac, outfile);
    outfile.flush();
    outfile.close();
  }
}
