#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_NUM                     5
#define SERVER_PORT              8888


int create_socket(void){
    int listen_fd = 0;
    int sock_fd = 0;
    int opt = 1;

    struct sockaddr_in server , client;

    socklen_t len = 0;
    int timep = 0;
    int ret = 0;

    listen_fd = socket(AF_INET ,SOCK_STREAM , 0);
    if(listen_fd < 0){
        perror("create socket fail\n");
        return -1;
    }

    ret = setsockopt(listen_fd,SOL_SOCKET , SO_REUSEADDR, &opt ,sizeof(opt));
    if(ret < 0){
        perror("set socket reuse addr failed\n");
        return -1;
    }

    bzero(&server , sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    len = sizeof(struct sockaddr);

    ret = bind(listen_fd,(struct sockaddr *)&server ,len);
    if(ret < 0){
        perror("bind failed\n");
        return -1;
    }

    listen(listen_fd , MAX_NUM);

    return listen_fd;
}


int main(int argc , char *argv[])
{
    int listen_fd = 0;
    int sock_fd = 0;

    struct sockaddr_in server ,client ;
    socklen_t len = 0;

    fd_set global_rdfs,current_rdfs;

    int bytes = 0;
    int maxfd = 0;
    int i = 0;
    int ret = 0;
	int client_fd[FD_SETSIZE];
	
    char buf[128] = {0};

    len = sizeof(struct sockaddr_in);

    listen_fd = create_socket();
    FD_ZERO(&global_rdfs);
    FD_SET(listen_fd,&global_rdfs);
    maxfd = listen_fd ;
	
	for(i = 0;i < FD_SETSIZE;i++){
		client_fd[i] = -1;
	}

    while(1){
        current_rdfs = global_rdfs;
        ret = select(maxfd + 1,&current_rdfs,NULL,NULL,NULL);
        if(ret < 0 ){
            perror("select failed\n");
            return -1;
        }
		
	if(FD_ISSET(listen_fd ,&current_rdfs)){
			
	    sock_fd = accept(listen_fd ,(struct sockaddr *)&client,(socklen_t *)&len);
            if(sock_fd < 0){
		perror("accept failed\n");
                return -1;
             }
                FD_CLR(i,&current_rdfs);
		maxfd = maxfd > sock_fd ? maxfd:sock_fd;
                FD_SET(sock_fd, &global_rdfs);
				
				for(i = 0;i < maxfd;i++){
					if(-1 == client_fd[i]){
						client_fd[i] = sock_fd;
						break;
					}	
				}	
		}

        for(i = 0;i <= maxfd ;i++){
			
		if(-1 == client_fd[i]){
			continue;
		}

            if(FD_ISSET(client_fd[i],&current_rdfs)){
                printf("read socket:%d\n",client_fd[i]);

                bytes = recv(client_fd[i],buf,128,0);
                if(bytes < 0){
                    perror("recv failed\n");
                    return -1;
                }else if(bytes == 0){
                    FD_CLR(client_fd[i],&global_rdfs);
                    close(client_fd[i]);
					client_fd[i] = -1;
                    printf("The client disconnects \n");
                    continue;
                }

                printf("buf:%s\n",buf);
			send(client_fd[i], buf, strlen(buf), 0);
				

            }
	}

    }


    return 0;
}
