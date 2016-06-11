#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int arge,char *argc[]){
//文件标识符
int opb = 0;
int opf = 0;

//存储读出来的文件内容
char showF = 0;

//判断输入是否正确
if(arge < 3){
	printf("error\n");
}else{
	opb = open(argc[1],O_CREAT | O_RDONLY,0777);//打开文件，如果没有则创建文件
	if(opb == -1){//判断打开是否失败
		printf("open error \n");
		return -1;
	}else{
		printf("succeed\n");
	}
}

	
	opf = open(argc[2],O_CREAT | O_RDWR | O_APPEND,0777);//打开或者创建第二个文件

	if(opf == -1){
		printf("open error \n");
		return -1;
	}else{
		printf("succeed open\n");
	}

	lseek(opb,0,SEEK_SET);//将文件读写指针调整到文件的开头
	while(read(opb,&showF,1)){
		int g = write(opf,&showF,1);//进行写操作
		if(g == -1){
			printf("write error\n");
			break;
		}
	}

close(opb);
close(opf);

return 0;
}
