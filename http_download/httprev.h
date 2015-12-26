#ifndef HTTPREV_H_INCLUDED
#define HTTPREV_H_INCLUDED

#define MAX 5120

struct http_receive{
    char * rev_buf;
    int length;
    int head_part_index;
};

int download(int sock_fd,const char *file_name);

struct http_receive * create_rec();

int receiver(int sock_fd,struct http_receive *rec,const char *file_name);

FILE *create_file(const char*file_name);

#endif // HTTPREV_H_INCLUDED
