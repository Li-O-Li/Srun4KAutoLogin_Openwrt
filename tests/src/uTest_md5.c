// unit test for MD5-HMAC
#include <stdio.h>
#include "../../includes/md5.h"
#include "../../includes/memBlock.h"
int main(int argc, char *argv[])
{
    char * data = "testpassword";
	char * key = "c7a0c3f3e842b998cc4aabd6840b7ed8e5d62b5e21c93e54e9d08fc3e6070dbb";
	
	memBlock * out = hmac_md5(data , key);
	memBlockprintAsString(out);
    freeMemBlock(out);
	return 0;
}