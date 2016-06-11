#ifndef HTTPREV_H_INCLUDED
#define HTTPREV_H_INCLUDED

#define MAX 1024

struct http_receive{
    char * rev_buf;
    int length;
};

struct xmlVal{
    char version[16];
    char time[16];
    char necessity[8];
    char hash_md5[38];
    char name[64];
};

int download(int sock_fd);

struct http_receive * create_rec();

int receiver(int sock_fd,struct http_receive *rec);

FILE *create_file(const char*file_name);

int get_val(struct xmlVal*val,const char*context);

#endif // HTTPREV_H_INCLUDED
