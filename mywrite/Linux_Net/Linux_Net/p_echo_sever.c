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
	int socket_fd = 0;

	pid_t pid = 0;

	struct sockaddr_in client;

	socklen_t len = sizeof(struct sockaddr);
	
	listen_fd = create_socket();
	if(listen_fd < 0){
		perror("create listen_fd error");
		return -1;
	}

	while(1){
		socket_fd = accept(listen_fd , (struct sockaddr *)&client , &len);

		pid = fork();
		if(pid < 0){
			perror("fork error");
		}else if(pid == 0){
			char buf = 0;

			int bytes = 0;

			while(1){
				bytes = recv(socket_fd,&buf,1,0);
				if(bytes < 0){
					perror("recv failed\n");
                    			return -1;
				}else if(bytes == 0){
					close(socket_fd);
					printf("The client disconnects \n");
					return 0;
				}
				send(socket_fd,&buf,1,0);
			}

		}else {
			close(socket_fd);
			continue;
		}

	}

	return 0;
}
