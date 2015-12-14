#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int sock_fd = 0;
int over = 1;

void *input_pth(void *arg){
	char in[128] = {0};
	char name[32] = {0};
	while(1){
		fgets(in,128,stdin);
		write(sock_fd,in,128);
		
		printf("请输入对方用户名\n");
		fgets(name,32,stdin);
		
		if(strncmp(in, "quit", 4) == 0){
			over = 0;
			break;
		}
		
		write(sock_fd,name,32);
		
		bzero(in,128);
		bzero(name,32);
	}
	
}

void *output_pth(void *arg){
	 char buf[12] = {0};
	 int bytes = 0;
	 
	 while(1){
		bytes = read(sock_fd,buf,12);
		if(bytes < 0 ){
			perror("读取出错");
			
			close(sock_fd);
			
			over = 0;
			break;
		}else if(bytes == 0){
			printf("服务端关闭了服务\n");
			
			close(sock_fd);
			
			over = 0;
			break;
		}
		printf("%s",buf);
		fflush(stdout);
		
		bzero(buf,12);  
	}
	 
}

int create_con(){
	struct sockaddr_in servaddr ;
	int er = 0;
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0){
        perror("套接字创建失败");
        return 0;
    }

    bzero(&servaddr ,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.44.130");
    servaddr.sin_port = htons(8888);

    er = connect(sock_fd , (struct sockaddr *)&servaddr ,sizeof(servaddr));
    if(er < 0){
        perror("连接出错\n");
        return 0;
    }
	return 1;
}

int main(int argc, char *argv[]){
    pthread_t w_pth = 0;
	pthread_t r_pth = 0;
	 
    int er = 0;

	char name[32] = {0};
	
	er = create_con();
	if(er == 0){
		return -1;
	}
	
	printf("请输入用户名\n");
	fgets(name,32,stdin);
	write(sock_fd,name,32);
	
	er = pthread_create(&w_pth,NULL,input_pth,NULL);
	if(er < 0){
		perror("write pthread create error");
		return -1;
	}
	
	er = pthread_create(&r_pth,NULL,output_pth,NULL);
	if(er < 0){
		perror("read pthread create error");
		return -1;
	}
	
	
	
	while(1){
		sleep(2);
		
		if(over == 0){
			break;
		}	
	}
	
    return 0;
}
