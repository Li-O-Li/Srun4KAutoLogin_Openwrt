// unique Base64, has a different alpha table than ordinary ones
#ifndef _UBASE64_H
#define _UBASE64_H
#include "memBlock.h"


memBlock *ubase64Encode(const memBlock *pwd);

#endif