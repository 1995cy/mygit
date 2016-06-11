#include <stdio.h>

int main()
{
    FILE *file = NULL;

    char p[100] = {0};

    file = fopen("main.c","r");

    long len = lseek(file,0,SEEK_END) + 1;
    int num = 0;

    lseek(file,0,SEEK_SET);

    while((fgets(p,99,file) != NULL) && (feof(file) == 0) ){
        num++;
    }

    printf("the line number is %d\n",num);

    fclose(file);
    return 0;
}
