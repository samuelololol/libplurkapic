#include "libplurkapic.h"
static const char* plurk_url = "http://www.plurk.com/";
static const char* plurk_uri_request = "OAuth/request_token";
static const char* plurk_uri_access = "OAuth/access_token";
static const char* plurk_uri_verification = "http://www.plurk.com/OAuth/authorize?oauth_token=";

inline static char* s_concate(const char**, const char**); 
inline static int get_keysecret(const char*, char**, char**);
inline static int get_response_token(char*, char*, char**, char**);
inline static int get_verifier(char*, char*, char*, char*, char**);
inline static int get_access_token(char*, char*, char**, char**, char*);

inline static char* s_concate(const char** p1, const char** p2)
{
    size_t len1 = strlen(*p1);
    size_t len2 = strlen(*p2);
    size_t lennew = len1 + len2;
    char** newstring = malloc(sizeof(char) * (lennew + 1));
    memset(newstring, '\0', lennew + 1);

    newstring = strncpy(newstring, *p1, len1);
    newstring = strncat(newstring, *p2, len2);
    return newstring;
}

inline static int get_keysecret(const char* raw, 
                                char** key,
                                char** secret){
    int rc;
    char **rv = NULL;
    if (*key) free(*key);
    if (*secret) free(*secret);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG: HTTP-reply: %s\n",raw);
#endif
    rc = oauth_split_url_parameters(raw, &rv);
    //qsort(rv, rc, sizeof(char *), oauth_cmpstringp);
    if( rc>=2
            && !strncmp(rv[0],"oauth_token_secret=",18) 
            && !strncmp(rv[1],"oauth_token=",11)
      ){
        *key=strdup(&(rv[1][12]));
        *secret=strdup(&(rv[0][19]));
#if SAMUEL_DEBUG
        printf("SAMUEL_DEBUG: rv1: '%s'\trv0: '%s'\n", rv[1],rv[0]);
        printf("SAMUEL_DEBUG: key: '%s'\tsecret: '%s'\n",*key, *secret);
#endif
    }
    if(rv) free(rv);

    return 0;
}

inline static int get_response_token(char* req_key, 
                                     char* req_secret,
                                     char** res_key, 
                                     char** res_secret){
    int fail = 0;
    char *request_token_uri = s_concate(&(plurk_url), &(plurk_uri_request));

    char *postarg = NULL;
    char *req_url;
    char *reply;

    req_url = oauth_sign_url2(
              request_token_uri,  // url
              &postarg,           // postarg
              OA_HMAC,            // OAuthMethod
              "POST",             // HTTPMethod
              req_key,          // customer key
              req_secret,       // customer secret
              NULL,               // token key
              NULL);              // token secret

    reply = oauth_http_post(req_url,postarg);
    if (!reply){
        printf("HTTP request for an oauth request-token failed.\n");
        return (fail = 1);
    }
    else 
        fail = get_keysecret(reply, res_key, res_secret);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, res_key: '%s'\t res_secret: '%s'\n", *res_key, *res_secret);
#endif

    if(reply) free(reply);
    if(req_url) free(req_url);
    if(postarg) free(postarg);
    if(request_token_uri) free(request_token_uri);

    return fail;
}


inline static int get_access_token(char* req_key, 
                                   char* req_secret,
                                   char** res_key, 
                                   char** res_secret,
                                   char* verifier){
    
    int fail = 0;
    char *request_token_uri = s_concate(&(plurk_url), &(plurk_uri_access));

    char *postarg = NULL;
    char *req_url;
    char *reply;

    int verifier_len = strlen(verifier);
    char *verifier_perm = malloc(sizeof(char)*(15 + verifier_len + 1));
    memset(verifier_perm, 0, 15 + verifier_len + 1);
    verifier_perm = memcpy(verifier_perm, "oauth_verifier=", 15);
    verifier_perm = strncat(verifier_perm, verifier, verifier_len);


    //req_url = oauth_sign_url2(request_token_uri, // const char *url
    //        &postarg,                            // char **postarg 
    //        OA_HMAC,                             // OAuthMethod method
    //        "POST",                              // const char *http_method
    //        req_key,                           // const char *key
    //        req_secret,                        // const char *secret
    //        res_t_key,                        // const char *t_key
    //        res_t_secret                        // char *t_secret
    //        );
    // transfer oauth_sign_url2() in steps
    // example edited from: 
    // http://liboauth.sourceforge.net/tests_2oauthtest2_8c-example.html#a0
    int argc=0;
    char **argv=NULL;
    char *req_hdr = NULL;
    char *http_hdr= NULL;

    argc = oauth_split_url_parameters(request_token_uri, &argv);
#if SAMUEL_DEBUG
    if (1) {
        int i;
        for (i=0;i<argc; i++)
            printf("SAMUEL_DEBUG, before add:\n%d:%s\n", i, argv[i]);
    }
#endif
    // the most important step here!! add parameter
    oauth_add_param_to_array(&argc, &argv, verifier_perm);

#if SAMUEL_DEBUG
    if (1) {
        int i;
        for (i=0;i<argc; i++)
            printf("SAMUEL_DEBUG, after add:\n%d:%s\n", i, argv[i]);
    }
#endif


    oauth_sign_array2_process(&argc, &argv,
            NULL, //< postargs (unused)
            OA_HMAC,
            "GET", //< HTTP method (defaults to "GET")
            req_key, req_secret,//NULL, NULL);
            *res_key, *res_secret);

    req_hdr = oauth_serialize_url_sep(argc, 1, argv, ", ", 6);
    req_url = oauth_serialize_url_sep(argc, 0, argv, "&", 1);
    oauth_free_array(&argc, &argv);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, req_hdr: %s\n",req_hdr);
    printf("SAMUEL_DEBUG, req_url: %s\n",req_url);
#endif
    http_hdr = malloc(strlen(req_hdr) + 100);
    memset(http_hdr,0,100);
    sprintf(http_hdr, "Authorization: OAuth realm=\"\", %s", req_hdr);
#if SAMUEL_DEBUG
    printf("request URL=%s\n", req_url);
    printf("request header=%s\n\n", http_hdr);
#endif

    // POST HTTPMethod
    //reply = oauth_http_post2(req_url,postarg,http_hdr);
    // GET HTTPMethod
    reply = oauth_http_get2(req_url,postarg,http_hdr);

    if(!reply){
        printf("SAMUEL_DEBUG, HTTP request for an oauth request-token failed.\n");
        return (fail = 1);
    }
    else 
        fail = get_keysecret(reply, res_key, res_secret);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, res_key: '%s'\t res_secret: '%s'\n", *res_key, *res_secret);
#endif


    if(http_hdr) free(http_hdr);
    if(req_hdr) free(req_hdr);
    if(verifier_perm) free(verifier_perm);
    if(reply) free(reply);
    if(req_url) free(req_url);
    if(postarg) free(postarg);
    if(request_token_uri) free(request_token_uri);

    return fail;
}

inline static int get_verifier(char* req_key,
                               char* req_secret,
                               char* res_key, 
                               char* res_secret,
                               char** verifier){
    *verifier = malloc(sizeof(char) * 8);
    printf("verification link:\n '%s%s' \n", plurk_uri_verification, res_key);
    memset(*verifier, '\0', 8);
    scanf("%s",*verifier);
    // print the verification url 
    return 0;
}


// init function: get req_key and prepare the key storage
// argument1: the address of variables storing the API key/secret
// argument2: the address of variables will storing the final access token
int plurk_init(key_pair* req, key_pair* permanent){
    // prepare the varible for storing
    key_pair* tmp = malloc(sizeof(key_pair));
    tmp->key = NULL;
    tmp->secret = NULL;
    char* verifier_code = malloc(sizeof(char) * 8);

    // OAuth prcess procedure
    get_response_token(req->key, req->secret, &(tmp->key), &(tmp->secret));
    get_verifier(req->key, req->secret, tmp->key, tmp->secret, &verifier_code);
    get_access_token(req->key, req->secret, &(tmp->key), &(tmp->secret),
                     verifier_code);

    // store the key/secret information 
    permanent->key = tmp->key;
    permanent->secret = tmp->secret;
    return 0;
}
