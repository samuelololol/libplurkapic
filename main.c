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

//    /* do a simple test */
//    printf("SAMUEL_DEBUG, =========================================\n");
//    /* refer to get_access_token line: 135 */
//
//    char *request_token_uri = "http://www.plurk.com/APP/Timeline/plurkAdd";
//    char *request_qualifier = "qualifier=says";
//    char *request_content   = "content=testapic";
//
//    int inargc = 0;
//    int i;
//    char **inargv   = NULL;
//    char *req_hdr   = NULL;
//    char *req_url   = NULL;
//    char *http_hdr  = NULL;
//    char *reply     = NULL;
//    char *postarg   = NULL;
//
//    inargc = oauth_split_url_parameters(request_token_uri, &inargv);
//
//
//#if SAMUEL_DEBUG
//    printf("SAMUEL_DEBUG, before add parameters to array\n");
//    for (i=0;i<inargc; i++)
//        printf("%d:%s\n",i,inargv[i]);
//#endif
//
//    oauth_add_param_to_array(&inargc, &inargv, request_qualifier);
//    printf("SAMUEL_DEBUG, inargc:%d\n", inargc);
//    oauth_add_param_to_array(&inargc, &inargv, request_content);
//    printf("SAMUEL_DEBUG, inargc:%d\n", inargc);
//    
//
//
//#if SAMUEL_DEBUG
//    printf("SAMUEL_DEBUG, after add parameters to array\n");
//    for (i=0;i<inargc; i++)
//        printf("%d:%s\n",i,inargv[i]);
//#endif
//
//
//    //do something important here??
//    oauth_sign_array2_process(&inargc, &inargv,
//            NULL, //< postargs (unused)
//            OA_HMAC,
//            "POST", //< HTTP method (defaults to "GET")
//            request.key, request.secret,//NULL, NULL);
//            permanent.key, permanent.secret);
//
//
//    req_hdr = oauth_serialize_url_sep(inargc, 1, inargv, ", ", 100);
//    req_url = oauth_serialize_url_sep(inargc, 0, inargv, "&", 1);
//    //req_url = request_token_uri;
//    oauth_free_array(&inargc, &inargv);
//#if SAMUEL_DEBUG
//    printf("SAMUEL_DEBUG, req_hdr: %s\n", req_hdr);
//    printf("SAMUEL_DEBUG, req_url: %s\n", req_url);
//#endif
//
//    http_hdr = malloc(strlen(req_hdr) + 200);
//    memset(http_hdr,0,100);
//    sprintf(http_hdr, "Authorization: OAuth realm=\"\", %s", req_hdr);
//
//#if SAMUEL_DEBUG
//    printf("request URL=%s\n", req_url);
//    printf("request header=%s\n\n", http_hdr);
//#endif
//
//    reply = oauth_http_post2(req_url, &postarg, http_hdr);
//    if(!reply){
//        printf("SAMUEL_DEBUG, HTTP request for an oauth request-token failed.\n");
//    }
//    else 
//        printf("SAMUEL_DEBUG, reply: %s\n", reply);

    char* myqualifier = "says";

    plurk_post( &request
               ,&permanent
               ,mycont
               ,myqualifier);

    return 0;
}
