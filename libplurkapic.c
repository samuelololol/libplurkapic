#include "libplurkapic.h"
#include <stdarg.h>
#include <ctype.h>

const char* plurk_url = "http://www.plurk.com/";
const char* plurk_uri_request = "OAuth/request_token";
const char* plurk_uri_access = "OAuth/access_token";
const char* plurk_uri_verification = "http://www.plurk.com/OAuth/authorize?oauth_token=";
const char* plurk_uri_post = "APP/Timeline/plurkAdd";


inline static char* s_concate(const char**, const char**); 
inline static int get_keysecret(const char*, char**, char**);
inline static int get_response_token(char*, char*, char**, char**);
inline static int get_verifier(char*, char*, char*, char*, char**);
inline static int get_access_token(char*, char*, char**, char**, char*);
inline static void encode(const char *s, char *enc, char *tb);
inline static char* urlencode(const char* raw);



// url encode reference from: http://rosettacode.org/wiki/URL_encoding#C
static char rfc3986[256] = {0};
static char html5[256] = {0};
 
// url encode reference from: http://rosettacode.org/wiki/URL_encoding#C
inline void encode(const char *s, char *enc, char *tb)
{
    for (; *s; s++) {
        if (tb[*s]) sprintf(enc, "%c", tb[*s]);
        else        sprintf(enc, "%%%02X", *s);
        while (*++enc);
    }
}
 
// url encode reference from: http://rosettacode.org/wiki/URL_encoding#C
char* urlencode(const char* raw)
{
    unsigned char url[] = "http://foo bar/";
    //char enc[sizeof(url) * 3];
    char* enc = malloc(sizeof(raw) * 3);
 
    int i;
    for (i = 0; i < 256; i++) {
        rfc3986[i] = isalnum(i)||i == '~'||i == '-'||i == '.'||i == '_'
            ? i : 0;
        html5[i] = isalnum(i)||i == '*'||i == '-'||i == '.'||i == '_'
            ? i : (i == ' ') ? '+' : 0;
    }
 
    //encode(url, enc, rfc3986);
    encode(raw, enc, rfc3986);
    return enc;
}

/**
 * @brief string concate
 *
 * @param p1 address of string1
 * @param p2 address of string2
 *
 * @return address of newstring
 */
inline static char* s_concate(const char** p1, const char** p2)
{
    size_t len1 = strlen(*p1);
    size_t len2 = strlen(*p2);
    size_t len_newstring = len1 + len2;
    char* newstring = malloc(sizeof(char) * (len_newstring + 1));
    memset(newstring, '\0', len_newstring + 1);

    newstring = strncpy(newstring, *p1, len1);
    newstring = strncat(newstring, *p2, len2);
    return newstring;
}

/**
 * @brief static function for parse the key/secret info from raw data
 *
 * @param raw raw data
 * @param key key
 * @param secret secret
 *
 * @return error on 1
 */
inline static int get_keysecret(const char* raw, 
                                char** key,
                                char** secret)
{
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

/**
 * @brief Use API key/secret to retrive the resonse key/secret
 *
 * @param req_key API key
 * @param req_secret API secret key
 * @param res_key the address of the response key
 * @param res_secret the address of the response secret key
 *
 * @return error on 1
 */
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
              req_key,            // customer key
              req_secret,         // customer secret
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


/**
 * @brief Use API key, response key-pair and verification code 
 * to retrive the permanent key-pair
 *
 * @param req_key API key
 * @param req_secret API secret key
 * @param res_key the address of the response key
 * @param res_secret the address of the response secret key
 * @param verifier verification code
 *
 * @return error on 1
 */
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
    int i;
    char **argv=NULL;
    char *req_hdr = NULL;
    char *http_hdr= NULL;

    argc = oauth_split_url_parameters(request_token_uri, &argv);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, before add parameters to array\n");
    for (i=0;i<argc; i++)
        printf("%d:%s\n",i,argv[i]);
#endif

    // the most important step here!! add parameter
    oauth_add_param_to_array(&argc, &argv, verifier_perm);

#if SAMUEL_DEBUG
    printf("====\nSAMUEL_DEBUG, after add parameters to array\n");
    for (i=0;i<argc; i++)
        printf("%d:%s\n",i,argv[i]);
#endif

    //do something important here??
    oauth_sign_array2_process(&argc, &argv,
            NULL, //< postargs (unused)
            OA_HMAC,
            "POST", //< HTTP method (defaults to "GET")
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
    reply = oauth_http_post2(req_url,&postarg,http_hdr);
    // GET HTTPMethod
    //reply = oauth_http_get2(req_url,postarg,http_hdr);

    if(!reply){
        printf("SAMUEL_DEBUG, HTTP request for an oauth request-token failed.\n");
        return (fail = 1);
    }
    else 
        fail = get_keysecret(reply, res_key, res_secret);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, res_key: '%s'\t res_secret: '%s'\n", *res_key, *res_secret);
    printf("Samuel_DEBUG, final %x\n",res_key);
    printf("SAMUEL_DEBUG, postarg: %s\n",postarg);
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

/**
 * @brief Use API key pair and response key pair to generate the link to
 * authorize page to retrive the verification code
 *
 * @param req_key API key
 * @param req_secret API secret key
 * @param res_key response key
 * @param res_secret response secret key
 * @param verifier the address of the verification code
 *
 * @return error on 1
 */
inline static int get_verifier(char* req_key,
                               char* req_secret,
                               char* res_key, 
                               char* res_secret,
                               char** verifier){
    *verifier = malloc(sizeof(char) * 8);
    printf("verification link:\n '%s%s' \n", plurk_uri_verification, res_key);
    memset(*verifier, '\0', 8);
    scanf("%s",*verifier);
    return 0;
}

/**
 * @brief Use given API key to get the permanent key pair as the 
 * initialization step of plurk
 *
 * @param req the address of API key pair
 * @param permanent the address of permanent key pair
 *
 * @return error on 1
 */
int plurk_init(key_pair* req, key_pair* permanent){

    // prepare for response key
    key_pair* tmp = malloc(sizeof(key_pair));
    tmp->key = NULL;
    tmp->secret = NULL;

    // prepare for authorize verified code
    char* verifier_code = malloc(sizeof(char) * 8);

    // OAuth prcess procedure
    // 1. get response key
    // 2. get the verifier code
    // 3. get the permanent access key
    get_response_token(req->key, req->secret, &(tmp->key), &(tmp->secret));
    get_verifier(req->key, req->secret, tmp->key, tmp->secret, &verifier_code);
    get_access_token(req->key, req->secret, &(tmp->key), &(tmp->secret),
                     verifier_code);

    // store the permanent key/secret 
    permanent->key = tmp->key;
    permanent->secret = tmp->secret;

    // free the dynamic allocated space
    free(verifier_code);

    //free(tmp->key);      // << save for outer, no free
    //free(tmp->secret);   // << save for outer, no free
    return 0;
}

int plurk_api( key_pair* request
              ,key_pair* permanent
              ,const char* api_uri
              ,const char* invoke_method
              ,int arg_count, ...
              )
{




    char* request_uri = s_concate(&(plurk_url), &(plurk_uri_request));
    //char* post_qualifier_hd = "qualifier=";
    //char* post_qualifier    = s_concate( &(post_qualifier_hd)
    //                                    ,&(qualifier));
    //char* post_content_hd   = "content=";
    //char* post_content      = s_concate( &(post_content_hd)
    //                                    ,&(content));
    char* post_url = s_concate( &(plurk_url) ,&(api_uri));

    int argc = 0;
    int i = 0;
    char** argv    = NULL;
    char* req_hdr  = NULL;
    char* req_url  = NULL;
    char* http_hdr = NULL;
    char* reply    = NULL;
    char* postarg  = NULL;

    argc = oauth_split_url_parameters(post_url, &argv);

#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, before add parameters to array\n");
    for (i=0;i<argc; i++)
        printf("%d:%s\n",i,argv[i]);
#endif
    
    int arg_index;
    char* arg_from_valist;
    va_list vaarg;
    va_start(vaarg, arg_count);
    for (arg_index = 0; arg_index < arg_count; arg_index++) {
        arg_from_valist = va_arg(vaarg, char*);
        oauth_add_param_to_array(&argc, &argv, arg_from_valist);
#if SAMUEL_DEBUG
        printf("SAMUEL_DEBUG, arg_from_valist: %s\n", arg_from_valist);
        printf("SAMUEL_DEBUG, argc:%d\n", argc);
#endif
    }
    va_end(vaarg);

#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, before add parameters to array\n");
    for (i=0;i<argc; i++)
        printf("%d:%s\n",i,argv[i]);
#endif
    oauth_sign_array2_process(&argc, &argv,
            NULL, //< postargs (unused)
            OA_HMAC,
            "POST", //< HTTP method (defaults to "GET")
            request->key, request->secret,//NULL, NULL);
            permanent->key, permanent->secret);

    req_hdr = oauth_serialize_url_sep(argc, 1, argv, ", ", 100);
    req_url = oauth_serialize_url_sep(argc, 0, argv, "&", 1);
    //req_url = request_token_uri;
    oauth_free_array(&argc, &argv);
#if SAMUEL_DEBUG
    printf("SAMUEL_DEBUG, req_hdr: %s\n", req_hdr);
    printf("SAMUEL_DEBUG, req_url: %s\n", req_url);
#endif

    http_hdr = malloc(strlen(req_hdr) + 200);
    memset(http_hdr,0,100);
    sprintf(http_hdr, "Authorization: OAuth realm=\"\", %s", req_hdr);

#if SAMUEL_DEBUG
    printf("request URL=%s\n", req_url);
    printf("request header=%s\n\n", http_hdr);
#endif

    reply = oauth_http_post2(req_url, &postarg, http_hdr);
    if(!reply){
        printf("SAMUEL_DEBUG, HTTP request for an oauth request-token failed.\n");
    }
    else 
        printf("SAMUEL_DEBUG, reply: %s\n", reply);

    if (reply)    free(reply);
    if (req_hdr)  free(req_hdr);
    if (req_url)  free(req_url);
    if (http_hdr) free(http_hdr);
    if (postarg)  free(postarg);
    if (post_url) free(post_url);

    return 0;
}

int plurk_post( key_pair* request
               ,key_pair* permanent
               ,const char* invoke_method
               ,const char* content
               ,const char* qualifier
               )
{
    char* post_qualifier_h ="qualifier=";
    char* post_content_h   ="content=";

    char* encoded_string = urlencode(content);

    char* post_qualifier = s_concate(&(post_qualifier_h), &(qualifier));
    char* post_content   = s_concate(&(post_content_h), &(content));

    plurk_api( request
               ,permanent
               ,plurk_uri_post    // << API_URI
               ,invoke_method     // << RESTful method
               ,2                 // << arg_count
               ,post_content
               ,post_qualifier    // << ...
               );  

    return 0;
}


