#include <curl/curl.h>

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }
    CURL *curl = curl_easy_init();
    if(curl) {
        char *output = curl_easy_escape(curl, argv[1], 0);
        if(output) {
            printf("Encoded: %s\n", output);
            curl_free(output);
        }
    }
    return 0;
}
