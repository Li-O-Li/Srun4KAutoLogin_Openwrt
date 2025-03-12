#include "../../includes/getRequestBody.h"
int main(){
    char* res=getRequestBody("jQuery112402113838359927389_1716630587851", "testuser", "testpassword", "1.1.1.1",
                             "1721302805808", "e7bbe23cfc8c5f025a0eb9c3cad31d19a93a8c1c5ad6d90ea25cda199058b860",
                             "login", "1", "200", "1", "Windows 10", "Windows", "0");
    printf("%s", res);
    return 0;
}