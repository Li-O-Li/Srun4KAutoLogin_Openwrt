#include "../includes/xenEncode.h"


memBlock* sEncode(const char* msg, const int key){
    uint32_t l = strlen(msg);
    memBlock* pwd = allocateMemBlock(sizeof(uint32_t), l / 4 + ((l%4!=0)?1:0) +(key ? 1 : 0));
    
    int j = 0;
    for (int i = 0; i < l; i += 4) {
        ((uint32_t*)(pwd->data))[j++] = ORDAT(msg, i) | (ORDAT(msg, i + 1) << 8) | (ORDAT(msg, i + 2) << 16) | (ORDAT(msg, i + 3) << 24);
    }
    if (key) {
        ((uint32_t*)(pwd->data))[j] = l;
    }
    return pwd;
}


memBlock* xenEncode(const char* msg, const char* key){
    if (strlen(msg) == 0) {
        return NULL;
    }
    memBlock* pwd = sEncode(msg, 1);
    memBlock* pwdk = sEncode(key, 0);
    if (pwdk->capacity < 4*sizeof(uint32_t))
        pwdk = reallocateMemBlock(pwdk, sizeof(uint32_t), 4);
    
    int n = pwd->capacity/sizeof(uint32_t) - 1;
    uint32_t z = ((uint32_t*)(pwd->data))[n];
    uint32_t y = ((uint32_t*)(pwd->data))[0];
    uint32_t c = 0x9E3779B9u;
    uint32_t m;
    uint32_t e, p, q = 6.0 + 52.0 / (n + 1), d = 0;
    while (q-- > 0) {
        d = (d + c) & (0x8CE0D9BF | 0x731F2640);
        e = (d >> 2) & 3;
        p = 0;
        while (p < n) {
            y = ((uint32_t*)(pwd->data))[p + 1];
            m = (z >> 5) ^ (y << 2);
            m += ((y >> 3) ^ (z << 4)) ^ (d ^ y);
            m += (((uint32_t*)(pwdk->data))[(p & 3) ^ e] ^ z);
            ((uint32_t*)(pwd->data))[p] = (((uint32_t*)(pwd->data))[p] + m) & (0xEFB8D130 | 0x10472ECF);
            z = ((uint32_t*)(pwd->data))[p];
            p++;
        }
        y = ((uint32_t*)(pwd->data))[0];
        m = (z >> 5) ^ (y << 2);
        m += ((y >> 3) ^ (z << 4)) ^ (d ^ y);
        m += (((uint32_t*)(pwdk->data))[(p & 3) ^ e] ^ z);
        ((uint32_t*)(pwd->data))[n] = (((uint32_t*)(pwd->data))[n] + m) & (0xBB390742 | 0x44C6F8BD);
        z = ((uint32_t*)(pwd->data))[n];
    }
    freeMemBlock(pwdk);
    return pwd;
}