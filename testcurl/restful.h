#ifndef __RESTFUL_H__
#define  __RESTFUL_H__
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>


CURL* RESTFUL_INIT();
CURL* RESTFUL_CLEANUP(CURL*);
int RESTFUL_GET(CURL**, const char*);


#endif
