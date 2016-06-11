#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resolve.h"

int port_resolve(char *start,ushort *port){
    int i = 1;

    while(start[i] != 0){
        if(start[i] < '0' || start[i] > '9'){
            return -1;
        }else{
            *port = *port * 10 + (start[i] - '0');
            i++;
        }
    }
    return 0;
}

int str_resolve(char *url_name,char *host_name,ushort *port){
    char * cut_http = NULL;
    char * get_port = NULL;
    int er = 0;

    cut_http = strstr(url_name,"http://");
    if(cut_http != NULL){
        url_name += 7;
    }

        get_port = strstr(url_name,":");
        if(get_port != NULL){
            strncpy(host_name,url_name,(get_port - url_name));

            er = port_resolve(get_port,port);
            if(er < 0){
                perror("端口解析出错");
                exit(-1);
            }

        }else{
            strcpy(host_name,url_name);
            *port = 80;
        }

    return 0;
}
