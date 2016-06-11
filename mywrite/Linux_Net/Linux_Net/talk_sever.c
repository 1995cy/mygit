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

struct client{
	int fd;
	char name[64];

	struct client *next;
	struct client *pre;
};

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

void head_client(struct client *head){
    head->fd = -1;
    bzero(head->name,64);
    head->next = NULL;
    head->pre = NULL;
}

void add_client(struct client *head,int sock_fd,char *user_name){
	struct client *p ;
	p = head;
	struct client *user = malloc(sizeof(struct client));

	while(p->next != NULL){
		p = p->next;
	}

	p->next = user;

	user->fd = sock_fd;
	memcpy(user->name , user_name,64);
	user->pre = p;
	user->next = NULL;
}

int find_fd(const char* name,struct client *head){
	struct client *p = head;
	
	while(p != NULL){
		
		printf("%s\n",p->name);
		
		if(strcmp(name,p->name) == 0){
			return p->fd;
		}
		p = p->next;
	}
	return 0;
}

char *find_name(int fd,struct client *head){
	struct client *p = head;
	while(p != NULL){
		
		if(p->fd == fd){
			return p->name;
		}
		
		p = p->next;
	}
	
	return NULL;
}

void del_fd(struct client *head,const int fd){
	struct client *p = head;
	
	while(p->next != NULL){
		if(p->fd == fd){
			p->pre->next = p->next;
			p->next->pre = p->pre;
			free(p);
			break;
		}
	}
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

    struct client head;
    int client_fd[FD_SETSIZE];

    char user_name[64] = {0};
	char *talk_name = NULL;
    char talk[1024] = {0};

    head_client(&head);

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
				
				bytes = recv(sock_fd,user_name,64,0);
				add_client(&head,sock_fd,user_name);
				
				break;
			}	
		}	
	}

     for(i = 0;i <= maxfd ;i++){
			
		if(-1 == client_fd[i]){
			continue;
		}

            if(FD_ISSET(client_fd[i],&current_rdfs)){
                bytes = recv(client_fd[i],talk,1024,0);
				int num = recv(client_fd[i],user_name,64,0);
                if(bytes < 0 || num < 0){
                    perror("recv failed\n");
                    return -1;
                }else if(bytes == 0 || num == 0){
                    FD_CLR(client_fd[i],&global_rdfs);
					del_fd(&head,client_fd[i]);
                    close(client_fd[i]);
					client_fd[i] = -1;
                    printf("The client disconnects \n");
                    continue;
                }
				
				int talk_fd = find_fd(user_name,&head);
				printf("fd %d\n",talk_fd);
				
				if(talk_fd == 0){
					send(client_fd[i],"can not find this user\n",50,0);
				}else{
					talk_name = find_name(client_fd[i],&head);
					
					printf("name is %s",talk_name);
					
					if(talk_name == NULL){
						strcpy(talk_name,"unknow");
					}
					send(talk_fd,"from user ",20,0);
					send(talk_fd,talk_name,64,0);
					send(talk_fd,talk,1024,0);
				}
            }
		}

    }


    return 0;
}
