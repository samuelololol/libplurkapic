#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "restful.h"

static enum {
    MAX_BUF = 256
};

struct mystring {
    char* ptr;
    size_t len;
};

int RESTFUL_GET(CURL** curl,const char* url){
    CURLcode res;
    curl_easy_setopt(*curl, CURLOPT_URL, url);
    curl_easy_setopt(*curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(*curl);

    if (res != CURLE_OK){
        fprintf( stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return -1;
    }
    return 0;
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



// ==============================
static int wr_index;
static char wr_buf[MAX_BUF + 1];
static size_t write_data (void*, size_t, size_t, struct mystring*);

// CURL_WRITEFUNCTION
static size_t write_data (void* ptr, size_t size, size_t nmemb, struct mystring* s)
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

static void init_string(struct mystring *s)
{
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr = NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}
