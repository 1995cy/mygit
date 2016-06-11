#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "creat.h"

int create_socket(void){
    int listen_fd = 0;
    int sock_fd = 0;
    int opt = 1;

    struct sockaddr_in server , client;

    socklen_t len = 0;
    int timep = 0;
    int ret = 0;

    listen_fd = socket(AF_INET ,SOCK_STREAM , 0);                                                                   //创建监听套接字
    if(listen_fd < 0){
        perror("create socket fail\n");
        return -1;
    }

    ret = setsockopt(listen_fd,SOL_SOCKET , SO_REUSEADDR, &opt ,sizeof(opt));                  //注册套接字
    if(ret < 0){
        perror("set socket reuse addr failed\n");
        return -1;
    }

    bzero(&server , sizeof(server));
    server.sin_family = AF_INET;                                                                                                    //设置相关参数
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    len = sizeof(struct sockaddr);

    ret = bind(listen_fd,(struct sockaddr *)&server ,len);                                                      //绑定
    if(ret < 0){
        perror("bind failed\n");
        return -1;
    }

    listen(listen_fd , MAX_NUM);                                                                                            //进行监听,设置最大监听数

    return listen_fd;
}

int deal(int listen_fd){
    int epofd = 0;                                                      //epoll对象
    int fds = 0;                                                            //活跃连接对象
    int i = 0;
	int er = 0;
	int sock_fd = 0;
	int len = 0;

    struct epoll_event event;                                   //epoll事件
	struct epoll_event events[MAX_NUM];

	struct sockaddr_in client;

	len = sizeof(struct sockaddr_in);

    epofd =	epoll_create(MAX_NUM);								//创建epoll对象
	if(epofd < 0){
		perror("epoll创建失败");
		return -1;
	}

	fcntl(listen_fd,F_SETFL,O_NONBLOCK);                        //将监听套接字转化为非堵塞的监听套接字

	event.data.fd = listen_fd;                                              //设置epoll事件
	event.events = EPOLLIN;

	er = epoll_ctl(epofd,EPOLL_CTL_ADD,listen_fd,&event);       //添加epoll事件
	if(er < 0){
		perror("epoll_ctl出错");
		return -1;
	}

	while(1){
		fds = epoll_wait(epofd,events,MAX_NUM,-1);              //等待数据连接
		if(fds < 0){
			perror("epoll_wait出错");
			continue;
		}

		for(i = 0;i < fds;i++){
			if(events[i].data.fd == listen_fd){                                                                 //判断是否是新联接
				sock_fd = accept(listen_fd,(struct sockaddr *)&client,&len);                //接受连接
				if(sock_fd < 0){
					perror("有客户端连接失败");
					continue;
				}

                //将连接添加到事件中
				event.data.fd = sock_fd;
				event.events = EPOLLIN | EPOLLET;
				epoll_ctl(epofd,EPOLL_CTL_ADD,sock_fd,&event);

				printf("有新连接%d\n",sock_fd);

				continue;
			}else {
				er = rev_data(events[i].data.fd);
				if(er == 0){
					epoll_ctl(epofd,EPOLL_CTL_DEL,events[i].data.fd,&event);
					close(events[i].data.fd);
					continue;
				}
			}

		}

	}

}

int rev_data(int sock_fd){
	int num = 0;
	char buf[2048];

	num = recv(sock_fd,buf,2048,0);
	if(num == 0){
        return 0;
	}else if(num < 0){
	    perror("数据接收出错");
        return -1;
	}

printf("%s\n",buf);

	send(sock_fd,buf,2048,0);

    bzero(buf,2048);

	return 1;
}
