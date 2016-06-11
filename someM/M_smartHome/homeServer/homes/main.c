#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sendFile.h"

#include <sys/types.h>
#include <sys/socket.h>

/*
 *进行客户端的初始化操作,创建子进程来进行接收远程服务器传来的数据
 *host_name : 远程服务器的ip或者域名
 *port: 端口号
 *返回值:成功返回0;不成功返回-1
 */
int initialize(char *host_name,ushort port){
    int sock_fd = 0;
    pid_t fpid = 0;

   sock_fd = create_con(host_name,port);
    if(sock_fd < 0){
        return -1;
    }

    fpid = fork();
    if(fpid < 0){//创建子进程出错
        return -1;
    }else if(fpid == 0){//父进程,发送数据
        //6fecec79bbd80466df0eaf481268b6eafeccb66b
        /*{
  "nonce": 10086,
  "path": "/v1/device/",
  "method": "GET",
  "meta": {
    "Authorization": "token 000..."
  },
  "get": {},
  "post": {},
  "body": {},
}*/

char action[] = "{\"path\": \"/v1/user/login/\",\"method\": \"POST\",\"meta\": {\"email\": \"827170272@qq.com\",\"password\":\"hjh123456\",\"remember\":1}}";
//char action[] = ""
send(sock_fd,action,1024,0);
printf("发送成功\n");


    }else{//子进程,接收数据
        char buf[1024] = {0};
            recv(sock_fd,buf,1024,0);
        printf("%s\n",buf);

    }

    close(sock_fd);
    return 0;
}

/*
 *将端口转化为ushort类型
 *argv:字符串类型的端口号
 *返回值:返回ushort类型的端口号
 */
ushort port_get(char *argv){
    ushort port = 0;
    int i =0;

    while(argv[i] != 0){
        port = port * 10 + (argv[i] - '0');
        i++;
    }

    return port;
}

/*
 *主函数
 *argc:输入的参数的个数
 *argv:输入的参数
 */

int main(int argc,char*arg[])
{
    int er = -1;
    int count = 0;
    ushort port = 0;

    if(argc < 3){
   //     printf("参数输入不正确\n");
    }

char argv[] = "192.168.1.104";
port = 8888;


    //port = port_get(argv[2]);

//出错重新启动
    while(er < 0 && count < 10){
        er = initialize(argv,port);
        count++;
    }

    if(er < 0){
        perror("程序遇到严重问题且多次重启仍没法解决\n");
        return -1;
    }

   return 0;
}
