#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"

const char* url = "http://localhost:6543/tachikoma";

int main(){
    printf("start to surf\n");
    CURL *curl;

    struct RESTFUL_STRING *response;
    RESTFUL_STRING_INIT(&response);

    char* request = "this is requests";


    // init curl
    RESTFUL_INIT(&curl);

    // normal processing
    RESTFUL_POST(&curl, url, (void *) request, (void *)response);

    printf("response:\n%s", response->ptr);
    RESTFUL_STRING_DESTROY(&response);

    RESTFUL_CLEANUP(&curl);
    return 0;
}


