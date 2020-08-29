/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
#include "conio.h"

#define	WSVERS	MAKEWORD(2, 0)
#define BUF_LEN 2000
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library



void cat_message(char * buf, char *src) {
	strcat(buf, src);
	strcat(buf, "\n");
}

int main(int argc, char *argv[]) {
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock, ssock;		    /* master & slave sockets	      */
	WSADATA wsadata;
	const char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
	int	    alen;			        /* from-address length		        */
	char	*pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */
	char	buf[BUF_LEN + 1];   		    /* buffer for one line of text	*/
	char ip_buf[BUF_LEN + 1];
	char port_buf[BUF_LEN + 1];


	WSAStartup(WSVERS, &wsadata);						// 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
	// 返回：要监听套接字的描述符或INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));						// 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							// 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// 监听所有(接口的)IP地址。
	sin.sin_port = htons((u_short)atoi(service));		// 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(host to network，s-short 16位)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // 绑定监听的IP地址和端口号

	listen(msock, 5);                                   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理。
	printf("服务端启动成功\n\n");

	while (!_kbhit()) { 		                                   // 检测是否有按键,如果没有则进入循环体执行
		alen = sizeof(struct sockaddr);                         // 取到地址结构的长度
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // 如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号
		int rec_cc = recv(ssock, buf, BUF_LEN, 0);
		if (rec_cc == SOCKET_ERROR) {                        // 出错。其后必须关闭套接字sock
			printf("错误: %d.\n", GetLastError());
		} else if (rec_cc == 0) {                       // 对方正常关闭
			printf("服务端已关闭!\n");
		} else if (rec_cc > 0) {
			buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("收到消息：%s\n", buf);                         // 显示所接收的字符串
		}

		(void)time(&now);                                      // 取得系统时间
		pts = ctime(&now);                                      // 把时间转换为字符串
		pts[strlen(pts)-1] = '\0';
		strcat(buf, "\n时间：");
		cat_message(buf, pts);
		printf("收到时间：%s", pts);                                      // 显示发送的字符串

		//获取IP地址
		sprintf(ip_buf, "%u.%u.%u.%u",
		        fsin.sin_addr.S_un.S_un_b.s_b1,
		        fsin.sin_addr.S_un.S_un_b.s_b2,
		        fsin.sin_addr.S_un.S_un_b.s_b3,
		        fsin.sin_addr.S_un.S_un_b.s_b4);
		printf("客户端IP地址：%s\n", ip_buf);
		strcat(buf, "客户端IP地址：");
		cat_message(buf, ip_buf);

		//获取端口
		sprintf(port_buf, "%hu", fsin.sin_port);
		printf("客户端端口号：%s\n", port_buf);
		strcat(buf, "客户端端口：");
		cat_message(buf, port_buf);

		int cc = send(ssock, buf, strlen(buf), 0);
		if (cc == SOCKET_ERROR) {                         // 出错。其后必须关闭套接字sock
			printf("错误: %d.\n", GetLastError());
			closesocket(ssock);
			WSACleanup();
		} else if (cc == 0) {                       // 对方正常关闭
			printf("服务端已关闭!\n");
		} else if (cc > 0) {
			printf("发送成功！\n\n");
		}

		(void)closesocket(ssock);                              // 关闭连接套接字
	}
	(void)closesocket(msock);                                 // 关闭监听套接字
	WSACleanup();                                             // 卸载winsock library
	printf("按回车键继续...");
	getchar();										// 等待任意按键
	getchar();
}
