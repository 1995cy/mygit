#include <stdio.h>

int main(int argc,char *argv[]){
FILE *opb = NULL;
FILE *opf = NULL;

char  show[16] ={0};

if(argc != 3){
		printf("error\n");
		return -1;
	}
opb = fopen(argv[1],"r");
if(opb == NULL){
	printf("open error\n");
	return -2;
}

opf = fopen(argv[2],"w+");
if(opf == NULL){
	printf("open error\n");
}
	int i = 0;

	while(feof(opb) == 0){
		i++;
		fgets(show,16,opb);
		/*int fp = fputs(show,opf);

		if(fp == -1){
		printf("write error\n");
		return -3;
		}*/
	}
	printf("d%d\n",feof(opb));
	printf("%d\n",i);

return 0;
}
