#include <UnitTest++.h>

#include <fstream>

#include "../src/ToGraphvizWriter.h"

// these test always pass, unless exceptions occur

SUITE(ArticleToGraphvizWriterTests)
{
    TEST(WriteArticleWithMultipleLinks)
    {
        std::ofstream outfile("_artlinks.dot", std::ofstream::out | std::ofstream::trunc);
        ToGraphvizWriter atj;
        Article a("Farm");
        a.addLink(new Article("Animal"));
        a.addLink(new Article("Pig"));
        a.addLink(new Article("Equality"));
        atj.output(&a, outfile);
        outfile.flush();
        outfile.close();
    }

    TEST(WriteArticleCollectionDepthOne)
    {
        std::ofstream outfile("_artctree.dot", std::ofstream::out | std::ofstream::trunc);
        ToGraphvizWriter atj;
        ArticleCollection ac;

        auto farm = new Article("Farm");
        ac.add(farm);

        auto animal = new Article("Animal");
        farm->addLink(animal);
        ac.add(animal);

        auto pig = new Article("Pig");
        farm->addLink(pig);
        ac.add(pig);

        auto equa = new Article("Equality");
        farm->addLink(equa);
        ac.add(equa);

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

        auto farm = new Article("Farm");
        ac.add(farm);

        auto animal = new Article("Animal");
        farm->addLink(animal);
        ac.add(animal);

        auto pig = new Article("Pig");
        pig->setMarked(true);
        farm->addLink(pig);
        ac.add(pig);

        pig->addLink(animal);
        animal->addLink(pig);

        auto equa = new Article("Equality");
        farm->addLink(equa);
        ac.add(equa);

        atj.output(ac, outfile);
        outfile.flush();
        outfile.close();
    }

    TEST(WriteArticleCollectionDepthTwo)
    {
        std::ofstream outfile("_artc_deeptree.dot", std::ofstream::out | std::ofstream::trunc);
        ToGraphvizWriter atj;
        ArticleCollection ac;

        auto farm = new Article("Farm");
        ac.add(farm);

        auto animal = new Article("Animal");
        farm->addLink(animal);
        ac.add(animal);

        auto pig = new Article("Pig");
        farm->addLink(pig);
        ac.add(pig);

        auto equa = new Article("Equality");
        farm->addLink(equa);
        ac.add(equa);

        auto rights = new Article("Rights");
        equa->addLink(rights);
        ac.add(rights);

        auto cat = new Article("Cat");
        ac.add(cat);
        animal->addLink(cat);
        auto dog = new Article("Dog");
        ac.add(dog);
        animal->addLink(dog);

        atj.output(ac, outfile);
        outfile.flush();
        outfile.close();
    }
}
