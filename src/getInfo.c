#include "../includes/getInfo.h"
memBlock *getInfo(const char*msg, const char*key){
    memBlock* xenText = xenEncode(msg, key);
    memBlock* info = ubase64Encode(xenText);
    free(xenText);

    // add prefix
    char *prefix="{SRBX1}";
    reallocateMemBlock(info, sizeof(char), info->capacity+7);
    memmove((char *)info->data + 7, info->data, info->capacity - 7);
    memcpy(info->data, prefix, 7*sizeof(char));
    return info;
}