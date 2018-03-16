//! \file CommandLineParserBase.h

#ifndef WIKIWALKER_ACOMMANDLINEPARSER_H
#define WIKIWALKER_ACOMMANDLINEPARSER_H

#include <string>

namespace WikiWalker
{
  //! Abstract base class for parsing command line arguments
  class CommandLineParserBase
  {
  public:
    //! possible command line flags
    enum class CommandLineOptions {
      //! version
      Version,
      //! help
      Help,
      //! URL argument
      URL,
      //! cache option
      JsonCache,
      //! Graphviz output option
      DotOut,
      //! whether to use "deep mode"
      FetchDeep
    };

    //! virtual base class d'tor
    virtual ~CommandLineParserBase() = default;

    /*! Parses command line args and stores results internally.
     * May throw an exception in case of error
     * \param argc number of arguments
     * \param argv arguments as passed to main
     */
    virtual void parse(int argc, char** argv) = 0;

    /*! Query whether a specific flag was set on command line.
     * \param flag Command line switch
     * \return whether specified flag was set on command line
     */
    virtual bool hasSet(CommandLineOptions flag) = 0;

    /*! get argument of switch (if available)
     * \param option command line options
     * \returns value as string
     */
    virtual std::string getValue(CommandLineOptions option) = 0;

    /*! Print usage help
     */
    virtual void printHelp() = 0;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_ACOMMANDLINEPARSER_H
