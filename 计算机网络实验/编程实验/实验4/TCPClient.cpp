#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <direct.h>

#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary


unsigned __stdcall send_message(void * p) {
	SOCKET *pt = (SOCKET *)p;
	char send_buf[BUFLEN + 1]; //发送缓冲区
	while (1) {
		fgets(send_buf, BUFLEN, stdin);
		int size = strlen(send_buf) - 1;
		send_buf[size] = '\r';
		send_buf[size + 1] = '\n';
		int send_cc = send(*pt, send_buf, size + 2, 0);
		if (send_cc == SOCKET_ERROR) {                        // 出错。其后必须关闭套接字sock
			printf("发送失败! 错误: %d.\n", GetLastError());
		} else if (send_cc == 0) {                   // 对方正常关闭
			printf("发送失败! 服务端已关闭!\n");
		} else if (send_cc > 0) {
			printf("发送成功\n");
		}
	}
	return 1;
}

unsigned __stdcall rec_message(void * p) {
	int rec_cc;
	SOCKET *pt = (SOCKET *)p;
	char rec_buf[BUFLEN + 1]; //接收缓冲区

	while (rec_cc = recv(*pt, rec_buf, BUFLEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                      // 出错。其后必须关闭套接字sock
			printf("接收失败! 错误: %d.\n", GetLastError());
		} else if (rec_cc == 0) {                   // 对方正常关闭
			printf("接收失败! 服务端已关闭!\n");
		} else if (rec_cc > 0) {
			rec_buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("%s", rec_buf);            // 显示所接收的字符串
		}
	}
	(void)closesocket(*pt);           //关闭连接套接字
	return 0;
}

int ConnectToServer(char *argv[]) {
	const char	*host = argv[1];	    /* server IP to connect         */
	const u_short service_port = atoi(argv[2]);  	    /* server port to connect       */
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	DWORD recExit, sendExit;
	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本
	memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							  // 因特网地址簇
	sin.sin_addr.s_addr = inet_addr(host);                // 设置服务器IP地址(32位)
	sin.sin_port = htons(service_port);         // 设置服务器端口号

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议

	int ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // 连接到服务器
	if (ret == SOCKET_ERROR) {
		printf("服务器连接失败\n");
		getchar();
	} else {
		HANDLE rechThread = (HANDLE)_beginthreadex(NULL, 0, &rec_message, (void *)&sock, 0, NULL); //创建新线程
		HANDLE sendhThread = (HANDLE)_beginthreadex(NULL, 0, &send_message, (void *)&sock, 0, NULL);
		while (1) {
			GetExitCodeThread(rechThread, &recExit);
			GetExitCodeThread(sendhThread, &sendExit);
			if (recExit != STILL_ACTIVE || sendExit != STILL_ACTIVE) {
				break;
			}
		}
		CloseHandle(rechThread);
		CloseHandle(sendhThread);
	}
	closesocket(sock);                             // 关闭监听套接字
	WSACleanup();                                  // 卸载winsock library

	printf("退出！");
	getchar();										// 等待任意按键
	return 1;
}

int main(int argc, char* argv[]) {
	printf("TCPClient:\n");
	printf("Command: IPaddress IP Port\n");
	if (argc != 3) {
		printf("参数错误!\n");
		return -1;
	} else {
		printf("参数如下：\n%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
	}
	ConnectToServer(argv);
	return 0;
}