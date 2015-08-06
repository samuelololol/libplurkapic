#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"

//const char* url = "http://localhost:8001/tachikoma";
const char* url = "http://www.plurk.com/samuelololol";


int main(){
    printf("start to surf\n");
    CURL *curl;

    RESTFUL_STRING *response;
    RESTFUL_STRING_INIT(&response);


    // init curl
    RESTFUL_INIT(&curl);

    // normal processing
    RESTFUL_GET(&curl, url, (void *)response);

    printf("response:\n%s", response->ptr);
    RESTFUL_STRING_DESTROY(&response);

    RESTFUL_CLEANUP(&curl);
    return 0;
}


