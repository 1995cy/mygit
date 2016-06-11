#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{ 
int pid1,pid2;
if((pid1=fork()) == 0)
{
sleep(3);
printf("info1 from child process_1\n");
exit(0);
printf("info2 from child process_1\n");
}else
{
	if((pid2=fork()) == 0)
	{
	sleep(5);
	printf("info1 from child process_2\n");
	exit(0);
	}else
	{
	wait(NULL);
	wait(NULL);
	printf("info1 from parent process\n");
	exit(0);
	}
}

return 0;
}
