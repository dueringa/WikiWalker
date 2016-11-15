#include <string>
#include <sstream>
#include <json/json.h>
#include <iostream>
#include <map>

std::map<Json::ValueType, std::string> typeMapper  {
{   Json::ValueType::nullValue, "null"},
{   Json::ValueType::intValue, "int"},
{   Json::ValueType::uintValue, "uint"},
{   Json::ValueType::realValue, "real"},
{   Json::ValueType::stringValue, "string"},
{   Json::ValueType::booleanValue, "boolean"},
{   Json::ValueType::arrayValue, "array"},
{   Json::ValueType::objectValue, "object"}
};

class Myexcept : public std::exception
{
public:
    Myexcept(std::string message)
            : message(message){};
    ~Myexcept() throw(){};
    const char* what() const throw()
    {
        return message.c_str();
    }
private:
    std::string message;
};

void iterateover(const Json::Value & val, int level = 0)
{
    std::string foo(level, '-');

    // iterationtest
    for(const auto &value : val)
    {
        if(value.type() == Json::ValueType::objectValue)
        {
            std::cout << foo;
            for(auto &member : value.getMemberNames())
            {
                std::cout << member << ", ";
            }
            std::cout << std::endl;
            iterateover(value, ++level);
        }
    }
}

int main()
{
    std::string json = "{\"batchcomplete\":\"\",\"query\":{\"normalized\":[{\"from\":\"Amundsen_Basin\",\"to\":\"Amundsen Basin\"}],\"pages\":{\"37082520\":{\"pageid\":37082520,\"ns\":0,\"title\":\"Amundsen Basin\",\"links\":[{\"ns\":0,\"title\":\"Abyssal\"},{\"ns\":0,\"title\":\"Arctic\"},{\"ns\":0,\"title\":\"Arctic Ocean\"},{\"ns\":0,\"title\":\"Cenozoic\"},{\"ns\":0,\"title\":\"Eurasian Basin\"},{\"ns\":0,\"title\":\"Gakkel Ridge\"},{\"ns\":0,\"title\":\"Lomonosov Ridge\"},{\"ns\":0,\"title\":\"Nansen Basin\"},{\"ns\":0,\"title\":\"Roald Amundsen\"},{\"ns\":0,\"title\":\"Seafloor spreading\"},{\"ns\":0,\"title\":\"Wilkes Basin\"}]}}}}";
    Json::Reader reader;
    Json::Value document;

    // this works
    ////std::istringstream jsondatastream(json);
    ////jsondatastream >> document;

    // this, too
    // this gives better error reporting?
    bool success = reader.parse(json, document, false);

    if(!success) {
        throw Myexcept("Error parsing JSON");
    }

    bool needmoredata = !document.isMember("batchcomplete");
    //! TODO: fetch more

    auto next = document.get("query", Json::Value::nullSingleton());

    if (next == Json::Value::nullSingleton()) {
        throw Myexcept("Error parsing JSON - no query result");
    }

    next = next.get("pages", Json::Value::nullSingleton());

    if (next == Json::Value::nullSingleton()) {
        throw Myexcept("Error parsing JSON - no pages result");
    }
    
    iterateover(document);
}
