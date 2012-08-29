#include "libfinchurk.h"
static const char* plurk_url = "http://www.plurk.com/"
static const char* plurk_uri_request = "OAuth/request_token";
static const char* plurk_uri_access = "OAuth/access_token";

// done
inline static char* s_concate(const char** p1, const char** p2); 
// not yet
inline static int get_response_token(char* req_key, char* req_secret,
                                     char** res_key, char** res_secret);
// not yet
inline static int get_access_token(char* req_key, char* req_secret,
                                   char** res_key, char** res_secret,
                                   char* verifier);




inline static char* s_concate(const char* p1, const char* p2)
{
    size_t len1 = strlen(p1);
    size_t len2 = strlen(p2);
    size_t lennew = len1 + len2;
    char* newstring = malloc(sizeof(char) * (lennew + 1));
    memset(newstring, '\0', length + 1);

    newstring = strncpy(newstring, p1, len1);
    newstring = strncat(newstring, p2, len2);
    return newstring;
}

int plurk_init(key_pair* req, key_pair* permanent){
    int fail = 0;
    char* req_key = strndup(req->key,strlen(req->key));
    char* req_secret = strndup(req->secret,strlen(req->secret));
    char* res_key = NULL;
    char* res_secret = NULL;
    char verifier[8];

    // 1.
    fail = get_response_token(req_key, req_secret,
                              &res_key, &res_secret);
    if(fail) return 1;
    printf("http://www.plurk.com/OAuth/authorize?oauth_token=%s\n",res_key);
    // 2.
    memset(verifier, '\0', 8);
    scanf("%s",verifier);
    // 3.
    fail = get_access_token(req_key, req_secret,
                            &res_key, &res_secret,
                            verifier);
    if(fail) return 1;
    permanent->key = strncpy(permanent->key, res->key, strlen(res->key));
    permanent->secret = strncpy(permanent->secret, res->secret,
                                strlen(res->secret));

    if(res_secret) free(res_secret);
    if(res_key) free(res_key);
    if(req_secret) free(req_secret);
    if(req_key) free(req_key);

    return 0;
}

