#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
    int sock_fd = 0;
    int bytes = 0;
    int er = 0;

    char buf[128] = {0};
    char in[128] = {0};

    struct sockaddr_in servaddr ;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0){
        perror("套接字创建失败");
        return -1;
    }

    bzero(&servaddr ,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.44.130");
    servaddr.sin_port = htons(8888);

    er = connect(sock_fd , (struct sockaddr *)&servaddr ,sizeof(servaddr));
    if(er < 0){
        perror("连接出错\n");
        return -1;
    }
while(1){
    fgets(in,128,stdin);

    write(sock_fd,&in,128);
    bytes = read(sock_fd,buf,128);
    if(bytes < 0 ){
        perror("读取出错");
	close(sock_fd);
        return -1;
    }else if(bytes == 0){
        printf("服务端没有发送数据或者服务端关闭了服务\n");
	close(sock_fd);
        return -1;
    }
    printf("%s",buf);

    bzero(buf,128);
    bzero(in,4);
}
    close(sock_fd);
    return 0;
}
