//! \file CurlUrlCreator.h

#ifndef CURLURLCREATOR_H
#define CURLURLCREATOR_H

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

    //! delete move constructor, because of CURL handle
    CurlUrlCreator(CurlUrlCreator&&) = delete;

    //! delete copy assignment, because of CURL handle
    CurlUrlCreator& operator=(const CurlUrlCreator&) = delete;

    //! delete move assignment, because of CURL handle
    CurlUrlCreator& operator=(CurlUrlCreator&&) = delete;

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
#endif  // CURLURLCREATOR_H
