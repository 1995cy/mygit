#include "xmlget.h"

#ifndef SELECT_H_INCLUDED
#define SELECT_H_INCLUDED

#define MAX_NUM                     5
#define SERVER_PORT              8888
#define MAX                              1024
#define MAX_LENGTH              4096

struct valget{
    char *context;
    int length;
};

struct S_file{
    int sock_fd;
    char *name;
};

int create_socket(void);

int con_select(void);

void deal(int *sock_fd,fd_set *);

struct valget *create_valget();

void free_val(struct valget *);

void close_socket(int *sock_fd,fd_set *);

int dowhat(struct valget*val,int *sock_fd,fd_set *global_rdfs);

int doupdate(char *context,int sock_fd);

int isnecessity(const char *server,const char *client);

void send_xmlval(struct xmlVal *val,int sock_fd);

void back_val(struct valget *context,const char *which,const char *val);

void back_valend(struct valget *context);

void send_file(struct xmlVal*,int sock_fd);

void *P_sendfile(void *arg);

#endif // SELECT_H_INCLUDED
