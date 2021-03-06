#ifndef __RESTFUL_H__
#define  __RESTFUL_H__
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>


typedef struct _RESTFUL_STRING {
    char* ptr;
    size_t len;
}RESTFUL_STRING;

void RESTFUL_STRING_INIT(RESTFUL_STRING**);
void RESTFUL_STRING_DESTROY(RESTFUL_STRING **);
int RESTFUL_INIT();
int RESTFUL_CLEANUP(CURL**);
int RESTFUL_GET(CURL** curl, const char* url, void* response);
int RESTFUL_POST(CURL** curl, const char* url, void* request, void* response);


#endif
