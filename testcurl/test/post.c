#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>
#include "restful.h"

const char* url = "http://localhost:6543/tachikoma";

int main(){
    printf("start to surf\n");
    CURL *curl;

    RESTFUL_STRING *response;
    RESTFUL_STRING_INIT(&response);

    //char* request = "this is requests";
    time_t current;
    current = time(NULL);
    if (current == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to compute the current time.\n");
        return EXIT_FAILURE;
    }
    char* c_time_string;
    c_time_string = ctime(&current);
    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        return EXIT_FAILURE;
    }
    printf("main: c_time_string: %s\n", c_time_string);


    // init curl
    RESTFUL_INIT(&curl);

    // normal processing
    RESTFUL_POST(&curl, url, (void *) c_time_string, (void *)response);

    printf("response:\n%s", response->ptr);
    RESTFUL_STRING_DESTROY(&response);

    RESTFUL_CLEANUP(&curl);
    return 0;
}


