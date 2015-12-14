#include <stdio.h>

int main(int arge,char *argc[]){
//文件标识符
FILE * opb = NULL;
FILE * opf = NULL;

//存储读出来的文件内容
char showF = 0;

//判断输入是否正确
if(arge < 3){
	printf("error\n");
}else{
	opb = fopen(argc[1],"r");
	if(opb == NULL){//判断打开是否失败
		printf("open error \n");
		return -1;
	}else{
		printf("succeed\n");
	}

}

	opf = fopen(argc[2],"w+");

	if(opf == NULL){
		printf("open error \n");
		return -1;
	}else{
		printf("succeed open\n");
	}

	fseek(opb,0,SEEK_SET);//将文件读写指针调整到文件的开头
	while(fread(&showF,1,1,opb) == 1){
		int g = fwrite(&showF,1,1,opf);//进行写操作
		if(ferror(opf) != 0){
			printf("write error\n");
			break;
		}
	}

fclose(opb);
fclose(opf);

return 0;
}
