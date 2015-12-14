#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define FIFO_PATH "/home/linux/myfifo"

//写入管道
int main(int arge,char *argv[])
{
    int fdw = 0;
    int fdr = 0;
    char cwrite[8] = {0};

    if(arge != 2){
        printf("参数输入错误\n");
        return -1;
    }

    if(mkfifo(FIFO_PATH,0666) < 0 && errno != EEXIST){
        printf("创建错误\n");
        return -2;
    }else{
        fdr = open(FIFO_PATH, O_CREAT | O_RDONLY,0666);
        fdw = open(argv[1],O_CREAT | O_WRONLY,0666);

        if(fdw > 0){

            if(fdr > 0){
                while(read(fdr,cwrite,7) > 0){
                    write(fdw,cwrite,7);
                    memset(cwrite,0,sizeof(cwrite));
                }


            }else{
              printf("管道打开失败\n");
              return -3;
            }

        }else{
            printf("%s文件打开失败\n",argv[1]);
            return -4;
        }
    }

close(fdw);
close(fdr);

    return 0;
}
