#include "../includes/getRequestBody.h"
// callback, action, username, password, ac_id, ip, chksum, info, n, type, os, name, double_stack, _
char* getRequestBody(char *callback_data, char* s_usrname, char* pwdPlain, char* ipAddr, char* timeStamp, char*token, char* iaction, char* iac_id, char* in, char* itype, char* ios, char* iname, char* idouble_stack){
    requestBody r;
    // direct assign
    char *fieldVal[] = {"callback", "action", "username", "password", "ac_id", "ip", "chksum", "info", "n", "type", "os", "name", "double_stack", "_"};
    for(int itr=0;itr<14;itr++)
        r.field[itr] = fieldVal[itr]; 
    r.value[callback] = callback_data;
    r.value[action] = iaction;
    r.value[username] = s_usrname;
    r.value[ac_id] = iac_id;
    r.value[ip] = ipAddr;
    r.value[n] = in;
    r.value[type] = itype;
    r.value[os] = ios;
    r.value[name] = iname;
    r.value[double_stack] = idouble_stack;
    r.value[_] = timeStamp;


    // password, chksum and info need calculation
    // pwd
    memBlock *pwdEncrypted = hmac_md5(pwdPlain, token);

    // info
    char *infoMsgParts[7] = {"{\"username\":\"", s_usrname, 
                            "\",\"password\":\"", pwdPlain,
                            "\",\"ip\":\"", ipAddr,
                            "\",\"acid\":\"1\",\"enc_ver\":\"srun_bx1\"}"};
	// allocate message string
    size_t msgLength = 0;
    for(int itr=0;itr<7;itr++)
        msgLength += strlen(infoMsgParts[itr]);
    char* infoMsg = (char *)malloc(msgLength+1);
    // assembly message string
    infoMsg[msgLength] = '\0';
    msgLength = 0;
	for (int itr = 0; itr < 7; itr++) {
		memcpy(infoMsg + msgLength, infoMsgParts[itr], strlen(infoMsgParts[itr]));
		msgLength += strlen(infoMsgParts[itr]);
	}
    // calculate info
    // printf("%s", infoMsg);
    memBlock * infoStr = getInfo(infoMsg, token);
    free(infoMsg);

    // chksum
    // sha1( token+usrname+ token+pwdEncrypted(no prefix)+ token+ac_id
    //      +token+ip +token+n +token+type +token+info
    int chksumMsgPartLength[7];
    chksumMsgPartLength[0] = strlen(s_usrname);
    chksumMsgPartLength[1] = pwdEncrypted->capacity-5;
    chksumMsgPartLength[2] = strlen(r.value[ac_id]);
    chksumMsgPartLength[3] = strlen(ipAddr);
    chksumMsgPartLength[4] = strlen(r.value[n]);
    chksumMsgPartLength[5] = strlen(r.value[type]);
    chksumMsgPartLength[6] = infoStr->capacity;
    msgLength = 0;
    for(int itr=0;itr<7;itr++)
        msgLength += chksumMsgPartLength[itr];
    char* chksumMsg = (char*)malloc(7*strlen(token) + msgLength + 1);
    
    chksumMsg[7*strlen(token) + msgLength] = '\0';
    char *chkSumMsgParts[7] = {s_usrname, ((char*)pwdEncrypted->data)+5, r.value[ac_id], ipAddr, r.value[n], r.value[type], infoStr->data};
    msgLength = 0;
    for(int itr=0;itr<7;itr++){
        memcpy(chksumMsg+msgLength, token, strlen(token));
        msgLength += strlen(token);
        memcpy(chksumMsg+msgLength, chkSumMsgParts[itr], chksumMsgPartLength[itr]);
        msgLength += chksumMsgPartLength[itr];
    }
	// printf("%s", chksumMsg);
    memBlock* chkSumStr = sha1(chksumMsg);
    free(chksumMsg);

    // final assembly
    // {->%7B }->%7D，
    reallocateMemBlock(pwdEncrypted, 1, pwdEncrypted->capacity + 1);
    reallocateMemBlock(infoStr, 1, infoStr->capacity + 1);
    reallocateMemBlock(chkSumStr, 1, chkSumStr->capacity + 1);
    *((char*)pwdEncrypted->data + pwdEncrypted->capacity - 1) = '\0';
    *((char*)infoStr->data + infoStr->capacity - 1) = '\0';
    *((char*)chkSumStr->data + chkSumStr->capacity - 1) = '\0';
    r.value[password] = (char*)pwdEncrypted->data;
    r.value[info] = (char*)infoStr->data;
    r.value[chksum] = (char*)chkSumStr->data;

    char* final = (char*)malloc(1024); // usually around 400~500 character
    msgLength = 0; // first vacant byte in result
    for(int itr=0; itr<14; itr++){
        if(itr!=0){
            final[msgLength]='&';
            msgLength++;
        }
        // execute replace while writing
        for(int j=0; j<strlen(r.field[itr]); j++){
            switch(r.field[itr][j]){
                case ' ': final[msgLength]='+'; msgLength++; break;
                case '/': final[msgLength]='%'; 
                            final[msgLength+1]='2';
                            final[msgLength+2]='F';
                            msgLength+=3;break;
                case '?': final[msgLength]='%';
                            final[msgLength+1]='3';
                            final[msgLength+2]='F';
                            msgLength+=3;break;
                case '#': final[msgLength]='%';
                            final[msgLength+1]='2';
                            final[msgLength+2]='3';
                            msgLength+=3;break;
                case '&': final[msgLength]='%';
                            final[msgLength+1]='2';
                            final[msgLength+2]='6';
                            msgLength+=3;break;
                case '=': final[msgLength]='%';
                            final[msgLength+1]='3';
                            final[msgLength+2]='D';
                            msgLength+=3;break;
                case '{': final[msgLength]='%';
                            final[msgLength+1]='7';
                            final[msgLength+2]='B';
                            msgLength+=3;break;
                case '}': final[msgLength]='%';
                            final[msgLength+1]='7';
                            final[msgLength+2]='D';
                            msgLength+=3;break;
                case '+': final[msgLength]='%';
                            final[msgLength+1]='2';
                            final[msgLength+2]='B';
                            msgLength+=3;break;
                default:  final[msgLength]=r.field[itr][j]; msgLength++; break;
            }
        }
		final[msgLength] = '=';
		msgLength++;
        for(int j=0; j<strlen(r.value[itr]); j++){
            switch(r.value[itr][j]){
                case ' ': final[msgLength]='+'; msgLength++; break;
                case '/': final[msgLength]='%'; 
                            final[msgLength+1]='2';
                            final[msgLength+2]='F';
                            msgLength+=3;break;
                case '?': final[msgLength]='%';
                            final[msgLength+1]='3';
                            final[msgLength+2]='F';
                            msgLength+=3;break;
                case '#': final[msgLength]='%';
                            final[msgLength+1]='2';
                            final[msgLength+2]='3';
                            msgLength+=3;break;
                case '&': final[msgLength]='%';
                            final[msgLength+1]='2';
                            final[msgLength+2]='6';
                            msgLength+=3;break;
                case '=': final[msgLength]='%';
                            final[msgLength+1]='3';
                            final[msgLength+2]='D';
                            msgLength+=3;break;
                case '{': final[msgLength]='%';
                            final[msgLength+1]='7';
                            final[msgLength+2]='B';
                            msgLength+=3;break;
                case '}': final[msgLength]='%';
                            final[msgLength+1]='7';
                            final[msgLength+2]='D';
                            msgLength+=3;break;
                case '+': final[msgLength]='%';
                            final[msgLength+1]='2';
                            final[msgLength+2]='B';
                            msgLength+=3;break;
                default:  final[msgLength]=r.value[itr][j]; msgLength++; break;
            }
        }

    }
    final[msgLength]='\0';
    freeMemBlock(pwdEncrypted);
    freeMemBlock(infoStr);
    freeMemBlock(chkSumStr);
    return final;
}