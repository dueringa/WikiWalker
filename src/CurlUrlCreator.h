//! \file CurlUrlCreator.h

#ifndef CURLURLCREATOR_H
#define CURLURLCREATOR_H

#include <string>
#include <unordered_map>
#include <curl/curl.h>

/*! Creates GET URLs with parameters.
 *  With the help of curl
 */
class CurlUrlCreator
{
public:
    //! Create a new instance, given a base URL
    CurlUrlCreator(std::string baseUrl);

    //! delete copy constructor, because of CURL handle
    CurlUrlCreator(const CurlUrlCreator&) = delete;

    //! delete copy assignment, because of CURL handle
    CurlUrlCreator& operator=(const CurlUrlCreator&) = delete;

    /*! Add GET parameters to URL.
     *  If parameter keys are specified multiple times, later occurrences
     *  overwrite earlier ones.
     *  \param param the parameter name
     *  \param value the parameter value - UNESCAPED!
     *               This will be done by curl. If you already escape
     *               it will be double-escaped.
     *  \return reference to self, so method chaining is possible.
     */
    CurlUrlCreator& addParameter(std::string param, std::string value);

    //! Reset parameters
    void reset();

    /*! Creates the URL
     *  \return Complete GET URL.
     */
    std::string buildUrl() const;

    ~CurlUrlCreator();

private:
    std::string _baseUrl;
    std::unordered_map<std::string, std::string> args;
    CURL* handle;

};

#endif // CURLURLCREATOR_H
