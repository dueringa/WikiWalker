//! \file CurlUrlCreator.h

#ifndef WIKIWALKER_CURLURLCREATOR_H
#define WIKIWALKER_CURLURLCREATOR_H

#include <map>
#include <string>
#include <unordered_map>

#include <curl/curl.h>

namespace WikiWalker
{
  /*! Creates GET URLs with parameters.
   *  With the help of curl
   */
  class CurlUrlCreator
  {
  public:
    /*! Create a new instance, given a base URL
     * \param baseUrl Base URL that will be used for adding parameters
     */
    explicit CurlUrlCreator(std::string baseUrl);

    //! delete copy constructor, because of CURL handle
    CurlUrlCreator(const CurlUrlCreator&) = delete;

    //! default move constructor
    CurlUrlCreator(CurlUrlCreator&&) = default;

    //! delete copy assignment, because of CURL handle
    CurlUrlCreator& operator=(const CurlUrlCreator&) = delete;

    //! default move assignment
    CurlUrlCreator& operator=(CurlUrlCreator&&) = default;

    /*! Add GET parameters to URL.
     *  If parameter keys are specified multiple times, later occurrences
     *  overwrite earlier ones.
     *  \param param the parameter name
     *  \param value the parameter value - UNESCAPED!
     *               This will be done by curl. If you already escape
     *               it will be double-escaped.
     *  \return reference to self, so method chaining is possible.
     */
    CurlUrlCreator& addParameter(const std::string& param,
                                 const std::string& value);

    /*! Add GET parameters to URL.
     *  \param params a map of keys and values.
     *                values have to be UNESCAPED!
     *                This will be done by curl. If you already escape
     *                it will be double-escaped.
     *  \return reference to self, so method chaining is possible.
     */
    CurlUrlCreator& addParameter(
        const std::map<const std::string, const std::string>& params);

    //! Reset parameters
    void reset();

    /*! Creates the URL
     *  \return Complete GET URL.
     */
    std::string buildUrl() const;

    ~CurlUrlCreator();

  private:
    std::string baseUrl_;
    std::unordered_map<std::string, std::string> args_;
    CURL* handle_;
  };
}  // namespace WikiWalker
#endif  // WIKIWALKER_CURLURLCREATOR_H
