#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>

int process_stdin()
{
    return 0;
}

int process_listend(int sockfd)
{
    int newfd;
    
    newfd = accept(sockfd, NULL, NULL);
    if (newfd < 0) {
        perror("accept");
        return -1;
    }
    
    printf("have a new connection...\n");
    return newfd;
}

int process_message(int sockfd)
{
    char buf[128];
    int ret;
    
    ret = recv(sockfd, buf, sizeof(buf)-1, 0);
    if (ret < 0) {
        perror("recv");
        return -1;
    }
    else if ( ret == 0 ){
        printf("close remote connection...\n");
        return 0;
    }
    else{
        buf[ret] = 0;
        printf("the recv is :%s\n",buf);
        return ret;
    }
}

static int create_tcp_server(short port)
{
    int sockfd;
    struct sockaddr_in self;
    int ret;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("socket");
        return -1;
    }
    
    memset(&self,0,sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port   = htons(port);
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = bind(sockfd,(struct sockaddr *)&self,sizeof(self));
    if(ret == -1){
        perror("bind");
        return -1;
    }
    
    listen(sockfd,5);
    printf("listen...\n");
    
    return sockfd;
}

int main(int argc,char *argv[])
{
    int listenfd;
    fd_set current_fds,bak_fds;
    int ret;
    int maxfds;
    int i;
    int fdtmp;
    
    // 创建监听描述符
    listenfd = create_tcp_server(8888);
    if (listenfd < 0) {
        perror("create tcp server failed!");
        exit(-1);
    }
    
    // 多路复用进行聊天 0 listend  newfd(3 4 5)
    FD_ZERO(&bak_fds);
    FD_SET(0,&bak_fds);
    FD_SET(listenfd,&bak_fds);
    maxfds = listenfd;
    
    while (1){
        current_fds = bak_fds;
    
        ret = select(maxfds+1, &current_fds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select:");
            close(listenfd);
            return -1;
        }
    
        for (i = 0; i <= maxfds; i++) {
            if( FD_ISSET(i,&current_fds) ){
                if (i == 0) {
                    process_stdin();
                }
                else if( i == listenfd ){
                    fdtmp = process_listend(listenfd);
                    if (fdtmp < 0) {
                        fprintf(stderr, "listenfd failed!\n");
                        break;
                    }
                    printf("the newfd is %d\n",fdtmp);
                    FD_SET(fdtmp,&bak_fds);
                    if (fdtmp > maxfds) {
                        maxfds = fdtmp;
                    }
                }
                else{
                    ret = process_message(i);
                    if (ret == 0) {
                        FD_CLR(i,&bak_fds);
                        close(i);
                    }
                }
            }
        }
    }
    
}











