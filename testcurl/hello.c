#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"


int main(){
    printf("start to surf\n");
    CURL *curl;
    struct mystring response;

    // init curl
    RESTFUL_INIT(&curl);

    // normal processing
    RESTFUL_GET(&curl, "http://localhost:8001/tachikoma", &response);

    RESTFUL_CLEANUP(&curl);
    return 0;
}


