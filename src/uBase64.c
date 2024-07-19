#include "../includes/uBase64.h"

memBlock *ubase64Encode(const memBlock *pwd_old){  
    char base64Alpha[] = "LVoJPiCN2R8G90yg+hmFHuacZ1OWMnrsSTXkYpUq/3dlbfKwv6xztjI7DeBE45QA";
	memBlock *pwd = copyMemBlock(pwd_old);

    int len = pwd->capacity;
    int x = len % 3;
    if (x)
        reallocateMemBlock(pwd, sizeof(char), len + (3 - x));

    int r_len = (len + 2) / 3 * 4;
    memBlock* r = allocateMemBlock(sizeof(char), r_len);
    ((char *)r->data)[r_len] = '\0';

    for (int i = 0; i < len; i += 3) {
        int a = ((unsigned char*)pwd->data)[i] << 16 | ((unsigned char*)pwd->data)[i+1] << 8 | ((unsigned char*)pwd->data)[i+2];
        ((char *)r->data)[i/3*4] = base64Alpha[a >> 18];
        ((char *)r->data)[i/3*4+1] = base64Alpha[(a >> 12) & 63];
        ((char *)r->data)[i/3*4+2] = base64Alpha[(a >> 6) & 63];
        ((char *)r->data)[i/3*4+3] = base64Alpha[a & 63];
    }

    if (x == 1) {
        ((char *)r->data)[r_len-1] = '=';
        ((char *)r->data)[r_len-2] = '=';
    } else if (x == 2) {
        ((char *)r->data)[r_len-1] = '=';
    }
	free(pwd);
    return r;
}