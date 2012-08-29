#ifndef __LIBFINCHURK_H__
#define __LIBFINCHURK_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oauth.h>
#include <json/json.h>

typedef struct {
    char* key;
    char* secret;
} key_pair;

int plurk_init(key_pair* req, key_pair* permanent);

// the following all are not done yet
int plurk_login(key_pair* permanent);
int plurk_logout(key_pair* permanent);
int plurk_post(key_pair* permanent, const char* content, const char* qualifier);

#endif
