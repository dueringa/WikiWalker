#include <string>
#include <sstream>
#include <json/json.h>

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

int main()
{
    std::string json = "{\"batchcomplete\":\"\",\"query\":{\"normalized\":[{\"from\":\"Amundsen_Basin\",\"to\":\"Amundsen Basin\"}],\"pages\":{\"37082520\":{\"pageid\":37082520,\"ns\":0,\"title\":\"Amundsen Basin\",\"links\":[{\"ns\":0,\"title\":\"Abyssal\"},{\"ns\":0,\"title\":\"Arctic\"},{\"ns\":0,\"title\":\"Arctic Ocean\"},{\"ns\":0,\"title\":\"Cenozoic\"},{\"ns\":0,\"title\":\"Eurasian Basin\"},{\"ns\":0,\"title\":\"Gakkel Ridge\"},{\"ns\":0,\"title\":\"Lomonosov Ridge\"},{\"ns\":0,\"title\":\"Nansen Basin\"},{\"ns\":0,\"title\":\"Roald Amundsen\"},{\"ns\":0,\"title\":\"Seafloor spreading\"},{\"ns\":0,\"title\":\"Wilkes Basin\"}]}}}}";
    Json::Reader reader;
    Json::Value document;
    std::istringstream jsondatastream(json);
    jsondatastream >> document;
    bool success = true; //reader.parse(json, document, false);

    if(!success) {
        throw Myexcept("Error parsing JSON");
    }

}
