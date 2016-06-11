#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "sendFile.h"

/*
 *创建连接和套接字
 *参数 网络地址:端口号
 *返回值:套接字
 */
int create_con(const char *host,ushort port){
    int sock_fd = -1;
    int er = 0;
    struct sockaddr_in server_addr;
    struct hostent *tmp;

    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd< 0){
        perror("没法创建socket");
        return -1;
    }

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    tmp = gethostbyname(host);
    if(tmp == NULL){
        perror("连接时域名解析出错");
        return -1;
    }
    memcpy(&server_addr.sin_addr,(struct in_addr *) tmp->h_addr,4);

    er = connect(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(er < 0){
        perror("连接失败");
        return -1;
    }else{
        printf("连接%s成功\n",host);
    }

    return sock_fd;
}
