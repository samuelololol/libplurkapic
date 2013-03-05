#include <stdio.h>
#include "libplurkapic.h"

key_pair request;
key_pair permanent;
int main(int argc, char const* argv[])
{
    permanent.key = NULL;
    permanent.secret = NULL;

    char* mycont;

    // input api key/secret
    request.key = strndup(argv[1],strlen(argv[1]));
    request.secret = strndup(argv[2],strlen(argv[2]));
    mycont = strndup(argv[3],strlen(argv[3]));
    
    printf("key: '%s'\t secret: '%s'\n", request.key, request.secret);
    printf("permanent key: '%s'\t secret: '%s'\n", permanent.key 
                                                 , permanent.secret);
    
    plurk_init(&request, &permanent);

    printf("key: '%s'\t secret: '%s'\n", request.key, request.secret);
    printf("permanent key: '%s'\t secret: '%s'\n", permanent.key
                                                 , permanent.secret);

    char* speak_content="content=";
    size_t len_cont_hd = strlen(speak_content);
    size_t len_cont_w  = strlen(mycont);
    size_t len_cont    = len_cont_hd + len_cont_w;
    char* newstring = malloc(sizeof(char) * (len_cont + 1));
    memset(newstring, '\0', len_cont + 1);

    newstring = strncpy(newstring, speak_content, len_cont_hd);
    newstring = strncat(newstring, mycont, len_cont_w);

    plurk_api( &request
               ,&permanent
               ,plurk_uri_post    // << API_URI
               ,"POST"            // << RESTful method
               ,2                 // << arg_count
               ,newstring         // << ...
               ,"qualifier=says");  

    return 0;
}
