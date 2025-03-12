// xEncode for Info
#ifndef _XENENCODE_H
#define _XENENCODE_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "memBlock.h"


// support function
#define ORDAT(msg, idx) ((strlen(msg)>idx)?(msg[idx]):'\0')

memBlock* sEncode(const char* msg, const int key);


// main function
memBlock* xenEncode(const char* msg, const char* key);

#endif