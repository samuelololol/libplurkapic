#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"

//static enum {
//    MAX_BUF = 256
//};
#define MAX_BUF 256

static int wr_index;
static char wr_buf[MAX_BUF + 1];
static size_t write_data (void* ptr, size_t size, size_t nmemb, struct RESTFUL_STRING* s);

// CURL_WRITEFUNCTION
static size_t write_data (void* ptr, size_t size, size_t nmemb, struct RESTFUL_STRING* s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);

    if (s->ptr == NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy((s->ptr + s->len), ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

void init_RESTFUL_STRING(struct RESTFUL_STRING *s)
{
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}


// ==============================
int RESTFUL_GET(CURL** curl,const char* url, void* response){
    CURLcode res;
    curl_easy_setopt(*curl, CURLOPT_URL, url);
    curl_easy_setopt(*curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(*curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (NULL != response)
    {
        curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(*curl, CURLOPT_WRITEDATA, response);
    }
    res = curl_easy_perform(*curl);
    if (res != CURLE_OK){
        fprintf( stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return -1;
    }
    return 0;
}
int RESTFUL_POST(CURL** curl,const char* url, void* request, void* response){
    CURLcode res;
    return 0;
}












void RESTFUL_STRING_INIT(struct RESTFUL_STRING **s)
{
    (*s) = (struct RESTFUL_STRING*) malloc(sizeof(struct RESTFUL_STRING));
    if (*s == NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    (*s)->ptr = malloc(1);
    (*s)->len = 0;
    if ((*s)->ptr == NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    (*s)->ptr[0] = '\0';
}

void RESTFUL_STRING_DESTROY(struct RESTFUL_STRING **s)
{
    if ( NULL != *s){
        if ( NULL != (*s)->ptr )
            free((*s)->ptr);
        free(*s);
    }
}


int RESTFUL_INIT(CURL** curl){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    *curl = curl_easy_init();
    return 0;
}
int RESTFUL_CLEANUP(CURL** curl){
    curl_easy_cleanup(*curl);
    return 0;
}


