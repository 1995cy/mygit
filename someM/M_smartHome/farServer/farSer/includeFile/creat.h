/*
 *该头文件用于创建于网络设置相关的函数及数据
 */
#ifndef CREAT_H_INCLUDED
#define CREAT_H_INCLUDED

#define MAX_NUM                      500                    //最大连接数
#define SERVER_PORT                 8888                   //服务器监听的端口号

//创建套接字
int create_socket(void);

//进行epoll处理
int deal(int listen_fd);

//接收数据
int rev_data(int sock_fd);

#endif // CREAT_H_INCLUDED
