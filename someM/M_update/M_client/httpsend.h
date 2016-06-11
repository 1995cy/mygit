#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#define MAX_PATH_FILENAME   1024
#define MAX   1024

struct http_head_context{
    char *context ;
    int length;
};

int create_con(const char *host,ushort port);

int head_send(int sock_fd);

struct  http_head_context * create_Shead();

void head_add_context(struct http_head_context *head,const char *request,const char *context);

void head_add_end(struct http_head_context *head);

int send_head(struct http_head_context *head,int sock_fd);

void head_free(struct http_head_context * head);

#endif // HTTP_H_INCLUDED
