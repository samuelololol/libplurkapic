#ifndef __RESTFUL_H__
#define  __RESTFUL_H__
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>


struct mystring {
    char* ptr;
    size_t len;
};

int RESTFUL_INIT();
int RESTFUL_CLEANUP(CURL**);
int RESTFUL_GET(CURL**, const char*, struct mystring*);


#endif
