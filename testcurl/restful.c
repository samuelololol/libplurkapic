#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

static enum {
    MAX_BUF = 256
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
static size_t write_data(void*, size_t, size_t, void*);

// CURL_WRITEFUNCTION
static size_t write_data (void* buffer, size_t size, size_t nmemb, void* userp)
{
    int segsize = size * nmemb;
    if (wr_index + segsize > MAX_BUF){
        *(int *)userp = 1;
        return 0;
    }
    memcpy((void *)&wr_buf[wr_index], buffer, segsize);
    wr_index += segsize;  // update index
    wr_buf[wr_index] = 0; // null therminate the buffer
    return segsize;
}

