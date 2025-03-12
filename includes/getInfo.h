// generate required "info" field
#ifndef _GETINFO_H
#define _GETINFO_H
#include "memBlock.h"
#include "xenEncode.h"
#include "uBase64.h"

memBlock *getInfo(const char*msg, const char*key);

#endif