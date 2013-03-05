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

    
    plurk_post( &request
               ,&permanent
               ,"POST"            // << invoke method
               //,"中文 測試 ^.<*y"
               ,mycont
               ,"says"            // << ...
               );

    return 0;
}
