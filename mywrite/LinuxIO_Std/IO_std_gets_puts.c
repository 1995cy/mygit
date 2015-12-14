#include <stdio.h>

int main(int argc,char *argv[]){
FILE *opb = NULL;
FILE *opf = NULL;

int show = 0;

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

	show = fgetc(opb);
	while(show != -1){
		
		if(fputc(show,opf) == -1){
		printf("write error\n");
		return -3;
		}
		show = fgetc(opb);
	}

return 0;
}
