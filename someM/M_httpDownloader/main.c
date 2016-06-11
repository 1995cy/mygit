#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resolve.h"
#include "httpsend.h"
//#include <unistd.h>

int http_downloader(char *url_name,char *file_name){
    int er = 0;
    char host_name[64] = {0};
    char path_filename[MAX_PATH_FILENAME] = {0};
    ushort port = 0;
    int sock_fd = -1;

    er = str_resolve(url_name,host_name,path_filename,&port);
    if(er){
        perror("地址解析出错");
        return -1;
    }

    printf("hostname is %s\n",host_name);
    printf("path_name is %s\n",path_filename);
    printf("port is %d\n",port);

    sock_fd = create_con(host_name,port);
    if(sock_fd < 0){
        return -1;
    }

    er = head_send(sock_fd,host_name,path_filename,port);
    if(er < 0){
        close(sock_fd);
        return -1;
    }

    er = download(sock_fd,file_name);
    if(er < 0){
        close(sock_fd);
        return -1;
    }
     close(sock_fd);
    return 0;
}

int main()
{
   // char url_buf[] = "http://www.baidu.com/index.html";
 //   char file_name[] = "index.html";

	//char url_buf[] = "http://west263.newhua.com/down/hfs2_3b285.rar";
	//char file_name[] = "hfs2_3b285.rar";

	//char url_buf[] = "http://vzidc3.newhua.com/down/PowerWordSetup2016.zip";
	//char file_name[] = "PowerWordSetup2016.zip";

	//char url_buf[] = "192.168.44.130/index.html";
	//char file_name[] = "index.html";


    int er = 0;

    er = http_downloader(url_buf,file_name);
    if(er){
        perror("下载文件失败,未知错误");
        exit(1);
    }

    printf("下载成功\n");
    return 0;
}
