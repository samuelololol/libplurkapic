#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"


int main(){
    printf("start to surf\n");
    CURL *curl;

    struct RESTFUL_STRING *response;
    RESTFUL_STRING_INIT(&response);


    // init curl
    RESTFUL_INIT(&curl);

    // normal processing
    RESTFUL_GET(&curl, "http://localhost:8001/tachikoma", (void *)response);

    printf("response:\n%s", response->ptr);

    RESTFUL_CLEANUP(&curl);
    return 0;
}


