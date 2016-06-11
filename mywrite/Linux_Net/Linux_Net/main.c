#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LISTEN_NUM          5

int main(int argc , char *argv[])
{
    int listen_fd = 0;
    int ret = 0;
    int opt = 1;
    int sock_fd = 0;

    char buf[128] = {0};

    socklen_t len = 0;

    struct sockaddr_in server , client;

    /*创建socket套接字，第一个为网络类型,
    第二个为套接字类型,就是tcp或者是udp,
    第三个默认为0*/
    listen_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(listen_fd < 0){//出错处理
        perror("创建套接字出错");
        return -1;
    }

    ret = setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt, sizeof(opt));
    if(ret < 0){
        perror("设置套接字出错");
        return -1;
    }

    bzero(&server , sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    len = sizeof(struct sockaddr);
    ret = bind(listen_fd ,(struct sockaddr *)&server , len);
    if(ret < 0){
        perror("绑定失败");
        return -1;
    }

    listen(listen_fd,MAX_LISTEN_NUM);

    while(1){
        sock_fd = accept(listen_fd , (struct sockaddr *)&client , &len);
        if(sock_fd < 0){
            perror("接收请求失败");
            return -1;
        }

    read(sock_fd,buf,128);

    printf("read is :%s\n",buf);
    //write(sock_fd , "hello world\n",13);
}

    close(sock_fd);

    printf("程序结束\n");
    return 0;
}
