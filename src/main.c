#include "../includes/getRequestBody.h"
#include <stdio.h>
int main(int argc, char* argv[]){
    // parameter 1 - callback handle (callback=)
    // parameter 2 - username
    // parameter 3 - password(plain)
    // parameter 4 - ipv4 address
    // parameter 5 - timestamp
    // parameter 6 - token(challenge returned by server)
    char * result = getRequestBody(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
    printf("%s", result);
    return 0;
}