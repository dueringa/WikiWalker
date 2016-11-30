//! \file ACommandLineParser.h

#ifndef _ACOMMANDLINEPARSER_H
#define _ACOMMANDLINEPARSER_H

#include <string>

//! Abstract base class for parsing command line arguments
class ACommandLineParser
{
public:
    virtual ~ACommandLineParser() {};

    /*! Parses command line args and stores results internally.
     * May throw an exception in case of error
     * \param argc number of arguments
     * \param argv arguments as passed to main
     */
    virtual void parse(int argc, char** argv) = 0;

    /*! Query whether a specific flag was set on command line.
     * \param flag Command line switch without leading dash
     * \return whether specified flag was set on command line
     */
    virtual bool hasSet(std::string flag) = 0;

    /*! get argument of switch (if available)
     * \param option command line switch without leading dash
     * \returns value as string
    */
    virtual std::string getValue(std::string option) = 0;

    /*! Print usage help
     */
    virtual void printHelp() = 0;
};

#endif // _ACOMMANDLINEPARSER_H
