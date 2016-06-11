
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

int process_stdin(int sockfd)
{
    char buf[128];
    int ret;
    
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf) - 1] = 0;
    
    if (strncmp(buf, "quit", 4) == 0) {
        printf("i am quit...\n");
        return -1;
    }
    
    ret = send(sockfd, buf, strlen(buf), 0);
    if (ret < 0) {
        perror("send");
        return -1;
    }
    
    printf("send %d success!\n",ret);
    return 0;
}

int process_recv(int sockfd)
{
    char buf[128];
    int ret;
    
    ret = recv(sockfd, buf, sizeof(buf) - 1, 0);
    if (ret == -1){
        perror("recv");
        return -1;
    }
    else if(ret == 0){
        printf("remote close...\n");
        return 0;
    }
    else{
        buf[ret] = 0;
        printf("the recv is %s\n",buf);
    }
    return ret;
}

int main(int argc, const char * argv[]) {
    int sockfd;
    struct sockaddr_in dest;
    short port;
    int ret;
    fd_set rdfds;
    int maxfds;
    
    if(argc != 3){
        fprintf(stderr, "please input ip port info!\n");
        exit(1);
    }
    
    port = (short)atoi(argv[2]);
    
    // 创建一个socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd){
        perror("socket");
        exit(-1);
    }
    
    memset(&dest,0,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_aton(argv[1],&dest.sin_addr);
    // 发起三次握手
    ret = connect(sockfd,(struct sockaddr *)&dest, sizeof(dest));
    if (ret == -1) {
        perror("connect...");
        exit(-1);
    }
    
    printf("Connect success!\n");
    
    while (1){
        FD_ZERO(&rdfds);
        FD_SET(0,&rdfds);
        FD_SET(sockfd,&rdfds);
        maxfds = sockfd;
    
        ret = select(maxfds + 1, &rdfds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select...");
            exit(-1);
        }
    
        if (FD_ISSET(0,&rdfds)) {
            if ( process_stdin(sockfd) < 0){
                fprintf(stderr, "send failed!\n");
                close(sockfd);
                break;
            }
        }
        if (FD_ISSET(sockfd,&rdfds)) {
            ret = process_recv(sockfd);
            if (ret <= 0) {
                close(sockfd);
                break;
            }
        }
    }
    
    return 0;
}
