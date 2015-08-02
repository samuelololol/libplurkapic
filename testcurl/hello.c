#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"


int main(){
    printf("start to surf\n");
    CURL *curl;

    // init curl
    RESTFUL_INIT(&curl);

    // normal processing
    RESTFUL_GET(&curl, "http://localhost:8001/tachikoma");

    RESTFUL_CLEANUP(&curl);
    return 0;
}


