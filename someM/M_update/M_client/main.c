#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resolve.h"
#include "httpsend.h"

int http_downloader(char *url_name){
    int er = 0;
    char host_name[64] = {0};
    ushort port = 0;
    int sock_fd = -1;

    er = str_resolve(url_name,host_name,&port);
    if(er){
        perror("地址解析出错");
        return -1;
    }



   sock_fd = create_con(host_name,port);
    if(sock_fd < 0){
        return -1;
    }


    er = head_send(sock_fd);
    if(er < 0){
        close(sock_fd);
        return -1;
    }
printf("hostname is %s\n",host_name);

//是否下载由用户和服务器共同决定
    er = download(sock_fd);
    if(er < 0){
        close(sock_fd);
        return -1;
    }

    close(sock_fd);
    return 0;
}

int main()
{
    char url_buf[] = "192.168.44.130:8888";

    int er = 0;

    er = http_downloader(url_buf);
    if(er){
        perror("下载文件失败,未知错误");
        exit(1);
    }

    printf("成功\n");
    return 0;
}
