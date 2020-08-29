/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <string.h>
#include "conio.h"
#include <string.h>

#define	WSVERS	MAKEWORD(2, 0)
#define BUF_LEN 2000
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library
#define MAX_CON 100

SOCKET ssock[MAX_CON];		//套接字数组
HANDLE hThread[MAX_CON];	 //子线程组
CRITICAL_SECTION cs;		 // 临界区

struct PARAM {
	int pos;
	char IP[BUF_LEN], port[BUF_LEN];
} param[MAX_CON];


void cat_message(char * buf, char *src) {
	strcat(buf, "\n");
	strcat(buf, src);
}

void send_to_all(char *buf) {
	for (int i = 0; i < MAX_CON; i++) {
		if (ssock[i] == NULL) continue;
		int send_cc = send(ssock[i], buf, strlen(buf), 0);
		if (send_cc == SOCKET_ERROR) {                         // 出错。其后必须关闭套接字sock
			printf("错误: %d.\n", GetLastError());
			EnterCriticalSection(&cs);				// 等待进入临界区
			closesocket(ssock[i]);
			ssock[i] = NULL;								//同一时刻只允许一个线程修改套接字组
			LeaveCriticalSection(&cs);			    // 离开临界区
		} else if (send_cc == 0) {                   // 对方正常关闭
			printf("服务端已关闭!\n");
		} else if (send_cc > 0) {
			printf("发送成功！\n");
		}
	}
}


unsigned __stdcall transmit(void *p) {
	char	buf[BUF_LEN + 1];   		    /* buffer for one line of text	*/
	int rec_cc;
	char	*pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */
	PARAM *pt = (PARAM*)p;

	while (rec_cc = recv(ssock[pt->pos], buf, BUF_LEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                        // 出错。其后必须关闭套接字sock
			printf("错误: %d.\n", GetLastError());
			getchar();
			break;
		} else if (rec_cc == 0) {                   // 对方正常关闭
			printf("服务端已关闭!\n");
			getchar();
			break;
		} else if (rec_cc > 0) {
			buf[rec_cc] = '\0';	                       // ensure null-termination
			(void)time(&now);                                      // 取得系统时间
			pts = ctime(&now);                                      // 把时间转换为字符串
			pts[strlen(pts) - 1] = '\0';
			strcat(buf, "\n时间：");
			cat_message(buf, pts);
			//printf("收到时间：%s", pts);                                      // 显示发送的字符串
			//获取IP地址
			//printf("客户端IP地址：%s\n",pt->IP);
			strcat(buf, "\n客户端IP地址：");
			cat_message(buf, pt->IP);
			//获取端口
			//printf("客户端端口号：%s\n", pt->port);
			strcat(buf, "\n客户端端口：");
			cat_message(buf, pt->port);
			printf("收到消息：%s\n", buf);                         // 显示所接收的字符串
			send_to_all(buf);
		}
	}


	EnterCriticalSection(&cs);				// 等待进入临界区
	(void)closesocket(ssock[pt->pos]);        // 关闭连接套接字
	ssock[pt->pos] = NULL;                   //重置
	CloseHandle(hThread[pt->pos]);           //关闭线程
	LeaveCriticalSection(&cs);				// 离开临界区
	return 0;
}

int main(int argc, char *argv[]) {
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock;		    /* master & slave sockets	      */
	WSADATA wsadata;
	const char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
	int	    alen = sizeof(struct sockaddr);			        /* from-address length		        */


	WSAStartup(WSVERS, &wsadata);						// 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
	memset(ssock, 0, sizeof(ssock));
	memset(&sin, 0, sizeof(sin));						// 从&sin开始的长度为sizeof(sin)的内存清0
	memset(&param, 0, sizeof(param));
	InitializeCriticalSection(&cs);				        // 初始化临界区
	sin.sin_family = AF_INET;							// 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// 监听所有(接口的)IP地址。
	sin.sin_port = htons((u_short)atoi(service));		// 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(host to network，s-short 16位)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // 绑定监听的IP地址和端口号

	listen(msock, MAX_CON);                                   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理。
	printf("服务端启动成功\n");

	while (!_kbhit()) { 		                                   // 检测是否有按键,如果没有则进入循环体执行
		int pos = 0;
		while (pos < MAX_CON && ssock[pos] != NULL) pos++;
		if (pos == MAX_CON) {
			printf("连接数已满\n");
		} else {
			param[pos].pos = pos;			//标记套接字位置
			ssock[pos] = accept(msock, (struct sockaddr *)&fsin, &alen);		//接受请求并建立连接
			strcpy(param[pos].IP, inet_ntoa(fsin.sin_addr));			//获取客户端IP
			sprintf(param[pos].port, "%hu", fsin.sin_port);      //获得客户端端口号
			hThread[pos] = (HANDLE)_beginthreadex(NULL, 0, &transmit, (void *)&param[pos], 0, NULL);	//创建新线程
		}
	}
	(void)closesocket(msock);                                 // 关闭监听套接字
	WSACleanup();                                             // 卸载winsock library
	DeleteCriticalSection(&cs);		                          // 删除临界区
	for (int i = 0; i < MAX_CON; i++) {
		if (ssock[i] != NULL)
			CloseHandle(hThread[i]);	//释放线程
	}
	printf("按回车键继续...");
	getchar();										// 等待任意按键
	getchar();
}