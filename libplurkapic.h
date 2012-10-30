/**
 * @file libplurkapic.h
 * @brief Plurk API in C
 * @author samuelololol <samuelololol[at]gmail.com>
 * @version 0.1
 * @date 2012-09-10
 */
#ifndef __LIBPLURKAPIC_H__
#define __LIBPLURKAPIC_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oauth.h>
#include <json/json.h>

/**
 * @brief key_pair, used to represent the key pair structure
 */
typedef struct {
    char* key;    ///< key value
    char* secret; ///< secret key value
} key_pair;       

int plurk_init(key_pair* req, key_pair* permanent);

// the following all are not done yet
// int plurk_login(key_pair* permanent);
int plurk_logout(key_pair* permanent);
//int plurk_post(key_pair* permanent, const char* content, const char* qualifier);
int plurk_api(key_pair* permanent,const char* content,const char* str);

#endif
