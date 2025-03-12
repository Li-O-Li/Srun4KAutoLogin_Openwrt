#include <stdio.h>
#include <string.h>
#include "../../includes/sha1.h"
#include "../../includes/md5.h"
#include "../../includes/getInfo.h"
#include "../../includes/memBlock.h"

int main(){
    char *infoMsg="{\"username\":\"testuser\",\"password\":\"testpassword\",\"ip\":\"172.17.160.219\",\"acid\":\"1\",\"enc_ver\":\"srun_bx1\"}";
    char *username = "testuser";
    char *pwd = "testpassword";
    char *token="d2d0a53134dd4823de8b2e8f0e7f24fa8ba419e78a9f60136db160b8fb6f0142";
	char *ac_id = "1";
    char *ip = "172.17.160.219";
    char *n = "200";
    char *type = "1";

    
    memBlock* hmd5 = hmac_md5(pwd, token);
    memBlock* info = getInfo(infoMsg, token);

    
    int size[14];
    size[0] = size[2] = size[4] = size[6] = size[8] = size[10] =size[12] = strlen(token);
    size[1] = strlen(username);
    size[3] = hmd5->capacity-5;
    size[5] = strlen(ac_id);
    size[7] = strlen(ip);
    size[9] = strlen(n);
    size[11] = strlen(type);
    size[13] = info->capacity;
    
    char *srtPos[14];
    srtPos[0] = srtPos[2] = srtPos[4] = srtPos[6] = srtPos[8] = srtPos[10] =srtPos[12] = token;
    srtPos[1] = username;
    srtPos[3] = (char *)hmd5->data+5;
    srtPos[5] = ac_id;
    srtPos[7] = ip;
    srtPos[9] = n;
    srtPos[11] = type;
    srtPos[13] = (char *)info->data;

    size_t totalSize = 0;
    for(int i=0; i<14; i++)
        totalSize += size[i];

    memBlock* chkstr = allocateMemBlock(sizeof(char), totalSize+1);
    
    int pastePlace = 0;
    for(int i=0; i<14; i++){
        memcpy(((char*)chkstr->data)+pastePlace, srtPos[i], size[i]);
        pastePlace += size[i];
    }
	((char*)chkstr->data)[totalSize] = '\0';

	// for (int i = 0; i < totalSize; i++)
	// 	putchar(*(((char*)chkstr->data) + i));
	// putchar('\n');
	memBlock* res = sha1((char*)chkstr->data);
    memBlockprintAsString(res);
    return 0;
}