#ifndef _SHA1_H
#define _SHA1_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include"../includes/memBlock.h"
#define rol(x,y) ((x<<y)|(x>>(32-y)))  //循环左移 

//一次循环过程，str为填充后的数据或是数据中的一部分 
void roundSha(unsigned char str[64],unsigned int h[5]);

//sha-1算法(网上的)
void sha1_old(unsigned char*input,long long len,unsigned char*output);

memBlock* sha1(char*input);
#endif