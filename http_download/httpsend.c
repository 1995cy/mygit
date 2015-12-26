#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "httpsend.h"

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

int head_send(int sock_fd,const char *host_name,const char *file_name,ushort port){
    int er = 0;
    struct http_head_context *head = NULL;
    char host[1024] = {0};

    snprintf(host,1024,"%s:%d",host_name,port);

    head = create_Shead();
    if(head == NULL){
        return -1;
    }

    head_add_filename(head,file_name);
    head_add_context(head,"Host: ",host);
    head_add_context(head,"Accept: ","*/*");
    head_add_context(head,"Connection: ","Close");
    head_add_end(head);

    printf("%s\n",head->context);

    er = send_head(head,sock_fd);
    if(er < 0){
        head_free(head);
        return -1;
    }

    head_free(head);

    return 0;
}

struct http_head_context * create_Shead(){
    struct http_head_context *head = NULL;
    head = (struct http_head_context *)malloc(sizeof(struct http_head_context));

    if(head == NULL){
        perror("http头信息结构体生成失败");
        return NULL;
    }

    head->context = malloc(MAX);
    if(head->context == NULL){
        perror("http头信息内容生成失败");
        return NULL;
    }
    memset(head->context,0,MAX);
    return head;
}

void head_add_filename(struct http_head_context * head,const char *file_name){
    int i = 0;
    char *g = "GET ";
    char *h = " HTTP/1.1\r\n";

    while(g[i] != 0){
        head->context[head->length++] = g[i++];
    }

    i = 0;
    while(file_name[i] != 0){
        head->context[head->length++] = file_name[i++];
    }

    i = 0;
    while(h[i] != 0){
        head->context[head->length++] = h[i++];
    }

}

void head_add_context(struct http_head_context *head,const char *request,const char *context){
    int i = 0;

    while(request[i] != 0){
        head->context[head->length++] = request[i++];
    }

    i = 0;
    while(context[i] != 0){
         head->context[head->length++] = context[i++];
    }

    head_add_end(head);

}

void head_add_end(struct http_head_context *head){
    char *g = "\r\n";
    int i = 0;

    while(g[i] != 0){
        head->context[head->length++] = g[i++];
    }

}

int send_head(struct http_head_context *head,int sock_fd){
    int er = 0;
    int num = 0;

    while(num < head->length){
        er = send(sock_fd,head->context,head->length - num,0);
        if(er < 0){
            perror("http请求发送失败");
            return -1;
        }
        num += er;
    }

    printf("请求发送成功\n");

    return 0;
}

void head_free(struct http_head_context * head){
    if(head->context != NULL){
        free(head->context);
    }

    free(head);
}
