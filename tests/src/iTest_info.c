// integrate test for getInfo
// result is the same as srun portal
#include <stdio.h>
#include "../../includes/memBlock.h"
#include "../../includes/getInfo.h"

int main(){
    char *msg = "{\"username\":\"testuser\",\"password\":\"testpassword\",\"ip\":\"172.17.160.219\",\"acid\":\"1\",\"enc_ver\":\"srun_bx1\"}";
    char *key = "c7a0c3f3e842b998cc4aabd6840b7ed8e5d62b5e21c93e54e9d08fc3e6070dbb";
    memBlock* info = getInfo(msg, key);
    memBlockprintAsString(info);
    freeMemBlock(info);
    return 0;
}