/*
带密钥的MD5算法库md5.h共有下列函数：

char* MDString (char *);
// 输入任意一个字符串，经过md5算法处理后，返回结果：一个定长（32个字符）字符串
char* MDFile (char *);
// 输入任意一个文件名，文件内容经过md5算法处理后，返回结果：一个定长（32个字符）字符串
char* hmac_md5(char* text, char* key);
// 输入任意一个字符串text,和一个用做密钥的字符串key,经过hmac_md5算法处理，返回处理结果：一个定长字符串（32个字符）
*/

#include <stdio.h>
#include <stdlib.h>
#include "md5.h"

int main(void)
{
char * text, *key ,*file;

text = "what do ya want for nothing?";
key = "Jefe";

printf("text: %s\n", text);
printf("key: %s\n", key);
printf("MD5 string: %s\n", hmac_md5(text, key));

file = "main.c";
printf("file: %s\n", file);
printf("MD5 string: %s\n",MDFile(file));

return 0;
}
