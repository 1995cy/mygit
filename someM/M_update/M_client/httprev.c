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
#include "md5.h"


int download(int sock_fd){
    int er = 0;
    struct http_receive *receive = NULL;

    receive = create_rec();
    if(receive == NULL){
        return -1;
    }

    er = receiver(sock_fd,receive);
    if(er < 0){
        return -1;
    }

    return 0;
}

FILE *create_file(const char*file_name){
    FILE *file = NULL;
    file = fopen(file_name,"w");
    while(file == NULL){
         file = fopen(file_name,"w");
    }
    return file;
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

        return rec;
}

int receiver(int sock_fd,struct http_receive *rec){
    int er =0;
    int i = 5;
    int flage = 1;
    char *cut = NULL;
    char buf[1024] = {0};
    FILE *file = NULL;
    struct xmlVal xmlval;

    memset(&xmlval,0,sizeof(struct xmlVal));

    struct xmlVal *val = &xmlval;

    while(1){
        if(flage == 1){
            er = recv(sock_fd,rec->rev_buf + rec->length,1024,0);
            if(er < 0){
                perror("接收失败");
                return -1;
            }

            cut = strstr(rec->rev_buf,"\r\n\r\n");
            if(cut != NULL){
                printf("%s\n",rec->rev_buf);

                flage = 0;

                //进行数据分析,判断是否必须升级或者提示升级或者无需升级
                er = get_val(val,rec->rev_buf);
                if(er < 0){
                    return -1;
                }else if(er == 1){
                    return 0;
                }

                if(strcmp(val->necessity,"NEED") == 0){
                    printf("有新版本,是否进行升级,输入y进行升级,其他任意键取消升级\n");
                    char *get= malloc(3);
                     get = fgets(get,2,stdin);
                    if(get[0] == 'y'|| get[0] == 'Y'){
                        free(get);
                        er = send(sock_fd,"OKUPADTE",10,0);
                        if(er < 0){
                            perror("请求发送失败");
                            return -1;
                        }
                        printf("下载文件中\n");
                    }else{
                        free(get);
                        printf("已取消升级\n");
                        return 0;
                    }
                }

                file = create_file(val->name);
                printf("下载文件中\n");
            }else{
                  perror("服务器端文件发送不完整");
                return -1;
            }

        }else {

            er = recv(sock_fd,buf,1024,0);

            if(er < 0){
                perror("接收失败");
                fclose(file);
                return -1;
            }
           // fputs(buf,file);
            fwrite(buf,1,er,file);
printf("%d\n",er);

            if(er< 1024){
                break;
            }
            memset(buf,0,1024);
        }
    }
    fclose(file);
    //对下载下来的文件进行验证
    char *md5 = MDFile(val->name);
    printf("%s\n",md5);
    if(strcmp(md5,val->hash_md5) != 0){
        perror("文件已经被修改");
        return -1;
    }

    change_upxml(val);

    return 0;
}

int get_val(struct xmlVal*val,const char*now){
    char *end = NULL;
    char *next_line = NULL;
    char *part = NULL;
    char which[12] = {0};

    int flag = 1;

    if(now[0] == 'N' && now[1] == 'O'){
        printf("已是最新版本,无需升级\n");
        return 1;
    }

    end = strstr(now,"\r\n\r\n");
    next_line = strstr(now,"\r\n");

    while(flag == 1){
            part = NULL;
            part = strstr(now,":");
            if(part == NULL){
                return -1;
            }
            memset(which,0,12);
            strncpy(which,now,part - now);

            if(strcmp(which,"version") == 0){
                strncpy(val->version,part + 1,next_line - part - 1);

            }else if(strcmp(which,"name") == 0){
                strncpy(val->name,part + 1,next_line - part - 1);

            }else if(strcmp(which,"time") == 0){
                strncpy(val->time,part + 1,next_line - part - 1);

            }else if(strcmp(which,"hash") == 0){
                strncpy(val->hash_md5,part + 1,next_line - part - 1);

            }else if(strcmp(which,"necessity") == 0){
                strncpy(val->necessity,part + 1,next_line - part - 1);
            }
            if(end == next_line){
                flag = 0;
            }else{
                now = next_line + 2;
                next_line = strstr(now,"\r\n");
            }
    }

    return 0;
}
