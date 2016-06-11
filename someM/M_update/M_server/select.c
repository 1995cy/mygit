#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "select.h"
#include "xmlget.h"

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

struct valget *create_valget(){
    struct valget *val = NULL;

    while(val == NULL){
         val = (struct valget *)malloc(sizeof(struct valget));
    }

    val->context = NULL;

    while(val->context == NULL){
        val->context = malloc(MAX_LENGTH);
    }
    memset(val->context,0,MAX_LENGTH);

    val->length = 0;
    return val;
};

void free_val(struct valget * val){
    if(val->context != NULL){
        free(val->context);
    }

      if(val != NULL){
        free(val);
    }
}

void close_socket(int *sock_fd,fd_set *global_rdfs){
            FD_CLR(sock_fd[0],global_rdfs);
                close(sock_fd[0]);
                sock_fd[0] = -1;
                printf("The client disconnects \n");
}

int con_select(void){
    int listen_fd = 0;
    int sock_fd = 0;

    struct sockaddr_in server ,client ;
    socklen_t len = 0;

    fd_set global_rdfs,current_rdfs;

    int maxfd = 0;
    int i = 0;
    int ret = 0;
	int client_fd[FD_SETSIZE];

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
                   deal(&client_fd[i],&global_rdfs);
            }
        }
    }
}

void deal(int *sock_fd,fd_set *global_rdfs){
    int bytes = 0;
    char buf[MAX] = {0};
    struct valget *val = create_valget();
    int i = 4;
    int er = 0;

    while(i--){
            bytes = recv(sock_fd[0],val->context + val->length,MAX,0);
            if(bytes < 0){
                perror("recv failed");
                close_socket(sock_fd,global_rdfs);
                break;
            }else if(bytes == 0){
                printf("client out\n");
                close_socket(sock_fd,global_rdfs);
                 free_val(val);
                return ;
            }else if(bytes < MAX){
                val->length += bytes;
                break;
            }
            val->length += bytes;
        }
        printf("%s\n",val->context);
       er = dowhat(val,sock_fd,global_rdfs);
       if(er < 0){
            printf("%d客户端请求处理失败",sock_fd[0]);
            close_socket(sock_fd,global_rdfs);
       }
        free_val(val);
}

int dowhat(struct valget*val,int *sock_fd,fd_set *global_rdfs){
    char *want = NULL;
    char *n = NULL;
    char *context = val->context;
    char action[16] = {0};
    int er = 0;

    if(strcmp(context,"OKUPADTE") == 0){
        struct xmlVal upval;
        getxmlval(&upval);

        send_file(&upval,sock_fd[0]);
        return 0;
    }

    want = strstr(context,"WANT ");
    if(context == NULL){
        return -1;
    }
    context += 5;
    n = strstr(context,"\r\n");
    if(n == NULL){
        return -1;
    }

    strncpy(action,context,n - context);
    context = n + 2;
    if(strcmp(action,"UPDATE") == 0){
       er = doupdate(context,sock_fd[0]);
       if(er < 0){
            return -1;
       }
    }
  //  close_socket(sock_fd,global_rdfs);
  return 0;
}

int doupdate(char *context,int sock_fd){
     char * version = NULL;
     char *n = NULL;
     char num_version[16] = {0};
     int er = 0;

     struct xmlVal xml_update;

    version = strstr(context,"Version: ");
    if(version == NULL){
        return -1;
    }
    context += 9;

    n = strstr(context,"\r\n");
    if(n == NULL){
        return -1;
    }

    strncpy(num_version,context,n - context);

    er = getxmlval(&xml_update);
    if(er < 0){
        return -1;
    }

    if(strcmp(xml_update.version,num_version) == 0){
        send(sock_fd,"NO\r\n\r\n",8,0);
        return 0;
    }

    er = isnecessity(xml_update.version,num_version);
    if(er > 0){
            printf("MUST\n");
            strcpy(xml_update.necessity,"MUST");
            send_xmlval(&xml_update,sock_fd);
            send_file(&xml_update,sock_fd);
    }else if(er == 0){
                strcpy(xml_update.necessity,"NEED");
                printf("NEED\n");
                send_xmlval(&xml_update,sock_fd);
    }else{
            return -1;
    }

    return 0;
}

void send_xmlval(struct xmlVal *val,int sock_fd){
    struct valget *context = NULL;

    context = create_valget();

    back_val(context,"version:",val->version);
    back_val(context,"name:",val->name);
    back_val(context,"necessity:",val->necessity);
    back_val(context,"hash:",val->hash_md5);
    back_val(context,"time:",val->time);
    back_valend(context);
    send(sock_fd,context->context,1024,0);
    printf("%s\n",context->context);
    free_val(context);
}

int isnecessity(const char *server,const char *client){
    char *s_n = NULL;
    char *c_n = NULL;
    char v_s[8] = {0};
    char v_c[8] = {0};

    s_n = strstr(server,".");
    if(s_n == NULL){
        return -1;
    }
    strncpy(v_s,server,s_n - server);

    c_n = strstr(client,".");
    if(c_n == NULL){
        return -1;
    }
    strncpy(v_c,client,c_n - client);

    if(strcmp(v_s,v_c) == 0){
        return 0;
    }

    return 1;
}

void back_val(struct valget *context,const char *which,const char *val){
    int i = 0;

    while(which[i] != 0){
        context->context[context->length++] = which[i++];
    }
    i = 0;
    while(val[i] != 0){
        context->context[context->length++] = val[i++];
    }

    back_valend(context);
}

void back_valend(struct valget *context){
    char *g = "\r\n";
    int i = 0;

    while(g[i] != 0){
        context->context[context->length++] = g[i++];
    }
}

void send_file(struct xmlVal* context,int sock_fd){
    pthread_t p_id = 0;
    struct S_file file;
    int er = -1;

    file.name = context->name;
    file.sock_fd = sock_fd;

    while(er < 0){
         er = pthread_create(&p_id,NULL,P_sendfile,(void *)&file);
    }
}

void *P_sendfile(void *arg){
        struct S_file *file = (struct S_file *)arg;
        FILE *fp = NULL;
        char buf[1024] = {0};
        int number = 0;
        int send_num = 0;
        int i = 0;

       fp =  fopen(file->name,"r");
       if(fp == NULL){
        perror("文件打开失败");
       }

        while(1){
           number =  fread(buf,1,1024,fp);
            send_num = send(file->sock_fd,buf,number,0);
printf("read:%d\nsend:%d\n",number,send_num);
            if(number < 1024){
                if(feof(fp) != 0){
                    printf("over\n");
                }else{
                    printf("read wrong\n");
                }
                break;
            }
    printf("%s\n",buf);
            memset(buf,0,1024);
        }
    fclose(fp);
}
