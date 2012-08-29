#include <stdio.h>
#include "libfinchurk.h"

key_pair request;
key_pair permanent;
permanent.key = NULL;
permanent.secret = NULL;

int main(int argc, char const* argv[])
{
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
