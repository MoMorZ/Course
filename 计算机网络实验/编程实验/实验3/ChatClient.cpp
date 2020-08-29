/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include "conio.h"

#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary
//#define TIME_BUF_LEN 30
//#define TEACHER
//#define LCJ
#define LOCAL
SOCKET	sock;		  	            /* socket descriptor	    	*/


/*------------------------------------------------------------------------
 * main - TCP client for TIME service
 *------------------------------------------------------------------------
 */

int send_message(char *buf) {
	int send_cc = send(sock, buf, strlen(buf), 0);
	if (send_cc == SOCKET_ERROR) {                        // 出错。其后必须关闭套接字sock
		printf("发送失败! 错误: %d.\n", GetLastError());
		getchar();
		return 0;
	} else if (send_cc == 0) {                         // 对方正常关闭
		printf("发送失败! 服务端已关闭!\n");
		getchar();
		return 0;
	} else if (send_cc > 0) {
		printf("发送成功\n");
		return 1;
	}
}


unsigned __stdcall rec_message(void * p) {
	int rec_cc;
	SOCKET *pt = (SOCKET *)p;
	char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/

	while (rec_cc = recv(*pt, buf, BUFLEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                      // 出错。其后必须关闭套接字sock
			printf("接收失败! 错误: %d.\n", GetLastError());
			getchar();
			return 0;
		} else if (rec_cc == 0) {                         // 对方正常关闭
			printf("接收失败! 服务端已关闭!\n");
			getchar();
			return 0;
		} else if (rec_cc > 0) {
			buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("收到的消息：%s\n", buf);            // 显示所接收的字符串
		}
	}
	(void)closesocket(*pt);           //关闭连接套接字
	return 0;
}


int ConnectToServer() {
#ifdef LOCAL
	const char	*host = "127.0.0.1";	    /* server IP to connect         */
	const char	*service = "50500";  	    /* server port to connect       */
#endif
#ifdef TEACHER
	const char	*host = "172.18.187.9";	    /* server IP to connect         */
	const char	*service = "50500";  	    /* server port to connect       */
#endif
#ifdef LCJ
	const char	*host = "172.26.32.13";	    /* server IP to connect         */
	const char	*service = "50500";  	    /* server port to connect       */
#endif
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本
	memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							  // 因特网地址簇
	sin.sin_addr.s_addr = inet_addr(host);                // 设置服务器IP地址(32位)
	sin.sin_port = htons((u_short)atoi(service));         // 设置服务器端口号

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议

	int ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // 连接到服务器
	if (ret == SOCKET_ERROR) {
		printf("服务器连接失败\n");
		getchar();
	} else {
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &rec_message, (void *)&sock, 0, NULL); //创建新线程
		while (1) {
			printf("请输入要发送的消息：\n");
			scanf_s("%s", buf, BUFLEN);
			if (strcmp(buf, "exit") == 0) {
				CloseHandle(hThread);
				printf("客户端退出\n");
				return 0;
			}
			if (0 == send_message(buf)) {
				CloseHandle(hThread);
				return 0;
			}
		}
	}



	closesocket(sock);                             // 关闭监听套接字
	WSACleanup();                                  // 卸载winsock library

	printf("按回车键继续...");
	getchar();										// 等待任意按键
	getchar();
	return 1;
}


int main(int argc, char *argv[]) {
	printf("客户端成功启动，退出请输入exit\n");
	while (ConnectToServer() != 0);
	printf("客户端正在退出，按回车确认\n");
	getchar();
	return 0;
}
