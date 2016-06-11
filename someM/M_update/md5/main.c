/*
����Կ��MD5�㷨��md5.h�������к�����

char* MDString (char *);
// ��������һ���ַ���������md5�㷨����󣬷��ؽ����һ��������32���ַ����ַ���
char* MDFile (char *);
// ��������һ���ļ������ļ����ݾ���md5�㷨����󣬷��ؽ����һ��������32���ַ����ַ���
char* hmac_md5(char* text, char* key);
// ��������һ���ַ���text,��һ��������Կ���ַ���key,����hmac_md5�㷨�������ش�������һ�������ַ�����32���ַ���
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
