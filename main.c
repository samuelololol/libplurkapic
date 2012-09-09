#include <stdio.h>
#include "libplurkapic.h"
#include "libplurkapic.h"<`0`>

key_pair request;
key_pair permanent;
int main(int argc, char const* argv[])
{
    permanent.key = NULL;
    permanent.secret = NULL;


    // input api key/secret
    request.key = strndup(argv[1],strlen(argv[1]));
    request.secret = strndup(argv[2],strlen(argv[2]));

    printf("key: '%s'\t secret: '%s'\n", request.key, request.secret);
    printf("permanent key: '%s'\t secret: '%s'\n", permanent.key, permanent.secret);
    
    plurk_init(&request, &permanent);

    printf("key: '%s'\t secret: '%s'\n", request.key, request.secret);
    printf("permanent key: '%s'\t secret: '%s'\n", permanent.key, permanent.secret);

    return 0;
}
