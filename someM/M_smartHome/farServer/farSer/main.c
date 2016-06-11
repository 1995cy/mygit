#include <stdio.h>
#include <stdlib.h>
#include "creat.h"

/*该函数提供服务
 *返回值  0代表无错误 ,-1代表出错
 *暂时没有参数
 */
int server_give(){
	int listenfd = 0;
	int er = 0;

	listenfd = create_socket();
	if(listenfd < 0){
		return -1;
	}

	er = deal(listenfd);
	if(er < 0){
		return -1;
	}

	return 0;
}

int main()
{
    int er = -1;                                                //标记函数是否出错
    int count = 0;                                            //重启计数

    while(er < 0 && count < 10){                //服务启动失败时重启
        er = server_give();

        count++;
    }

//重启无法解决问题,关闭程序向标准输出流中输出
    perror("程序发生多次重启仍无法解决的错误,抱歉退出");

    return 0;
}
