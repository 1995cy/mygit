#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    //打开文件
    if(argc < 2){
        printf("请输入文件名!\n");
        return 1;
    }else{
        int fd =0;
        //打开或者创建文件
        umask(0000);
        fd = open(argv[1],O_RDWR | O_CREAT,0666);
        if(fd == -1){
            printf("打开失败\n");
            return 1;
        }else {
            printf("打开成功\n");
            //读取文件
            char g;

            while(read(fd,&g,1) != 0){
                printf("%c",g);
            }

            //write();是写函数，参数和read（）一样
            //lseek();用于将文件内容读取指针进行偏移
            //lseek(fd,0,SEEK_END); //计算文件的长度

            int cl = close(fd);
            if(cl == -1){
                printf("关闭文件失败\n");
            }else{
                printf("关闭文件成功\n");
            }
        }
    }
    return 0;
}
