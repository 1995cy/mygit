#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "httprev.h"


int download(int sock_fd,const char *file_name){
    int er = 0;
    struct http_receive *receive = NULL;
    FILE *file = NULL;

    receive = create_rec();
    if(receive == NULL){
        return -1;
    }

    er = receiver(sock_fd,receive,file_name);
printf("%s\n",receive->rev_buf);
    if(er < 0){
        return -1;
    }

    return 0;
}

FILE *create_file(const char*file_name){
    FILE *file = NULL;
    file = fopen(file_name,"w");
    if(file == NULL){
        perror("打开或者创建文件失败");
        return NULL;
    }
}

struct http_receive * create_rec(){
        struct http_receive *rec = NULL;

        rec = (struct http_receive *)malloc(sizeof(struct http_receive));
        if(rec == NULL){
            perror("接收结构体空间分配失败");
            return NULL;
        }

        rec->rev_buf = malloc(MAX);
        if(rec->rev_buf == NULL){
            perror("接收结构体内部空间分配失败");
            return NULL;
        }
        memset(rec->rev_buf,0,MAX);
        rec->length = 0;
        rec->head_part_index = 0;

        return rec;
}

int receiver(int sock_fd,struct http_receive *rec,const char *file_name){
    int er =0;
    int i = 5;
    int flage = 1;
    char *cut = NULL;
    char buf[1024] = {0};
    FILE *file = NULL;

    while(1){

        if(flage == 1){
            er = recv(sock_fd,rec->rev_buf + rec->length,1024,0);
            if(er < 0){
                perror("接收失败");
                return -1;
            }

            cut = strstr(rec->rev_buf,"\r\n\r\n");
            if(cut != NULL){
                flage = 0;
                file = create_file(file_name);
                if(file == NULL){
                    return -1;
                }

                cut += 4;

                fputs(cut,file);
            }

            rec->length += er;
            if(er < 1024 && flage == 1){
                perror("服务器端文件发送不完整");
                return -1;
            }

        }else {
            er = recv(sock_fd,buf,1024,0);
printf("%d\n",er);
            if(er < 0){
                perror("接收失败");
                close(file);
                return -1;
            }

printf("\n%s\n\n\n\n",buf);
            fputs(buf,file);

            if(er < 1024){

                break;
            }

            memset(buf,0,1024);

        }

    }

    close(file);
    return 0;
}
