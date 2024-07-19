#ifndef _GETREQUESTBODY_H
#define _GETREQUESTBODY_H
#include <string.h>
#include <stdlib.h>
#include "memBlock.h"
#include "md5.h"
#include "getInfo.h"
#include "sha1.h"

typedef enum requestBodyFields{
    callback, action, username, password, ac_id, ip, chksum, info, n, type, os, name, double_stack, _
} requestBodyFields;

typedef struct requestBody{
    char *field[14];
    char *value[14];
} requestBody;

char* getRequestBody(char *callback_data, char* s_usrname, char* pwdPlain, char* ipAddr, char* timeStamp, char*token);
#endif