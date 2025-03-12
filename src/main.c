#include "../includes/getRequestBody.h"
#include <stdio.h>
int main(int argc, char* argv[]){
    // parameter 1 - callback handle (callback=)
    // parameter 2 - username
    // parameter 3 - password(plain)
    // parameter 4 - ipv4 address
    // parameter 5 - timestamp
    // parameter 6 - token(challenge returned by server)
    // parameter 7 - action
    // parameter 8 - ac_id
    // parameter 9 - n
    // parameter 10 - type
    // parameter 11 - os
    // parameter 12 - name
    // parameter 13 - double_stack
    char * result = getRequestBody(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13]);
    printf("%s", result);
    return 0;
}