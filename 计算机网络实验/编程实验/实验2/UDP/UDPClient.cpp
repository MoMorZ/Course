/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary

/*------------------------------------------------------------------------
 * main - TCP client for TIME service
 *------------------------------------------------------------------------
 */

int ConnectToServer() {
	const char	*host = "127.0.0.1";	    /* server IP to connect         */
	const char	*service = "50500";  	    /* server port to connect       */
	struct sockaddr_in sin;	        /* an Internet endpoint address	*/
	int sin_size = sizeof(sin);
	char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);       /* 启动某版本Socket的DLL        */

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons((u_short)atoi(service));    //atoi：把ascii转化为int. htons：主机序(host)转化为网络序(network), s--short
	sin.sin_addr.s_addr = inet_addr(host);           //如果host为域名，需要先用函数gethostbyname把域名转化为IP地址

	sock = socket(PF_INET,  SOCK_DGRAM, IPPROTO_UDP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，UDP协议
	printf("请输入要发送的消息：");
	scanf_s("%s", buf, BUFLEN);
	if (strcmp(buf, "exit") == 0) {
		printf("客户端退出\n");
		return 0;
	}

	int send_cc = sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&sin, sizeof(sin));
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
	}


	int rec_cc = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&sin, &sin_size);
	if (rec_cc == SOCKET_ERROR) {                        // 出错。其后必须关闭套接字sock
		printf("接收失败! 错误: %d.\n", GetLastError());
	} else if (rec_cc == 0) {                     // 对方正常关闭
		printf("接收失败! 服务端已关闭!\n");
	} else if (rec_cc > 0) {
		buf[rec_cc] = '\0';	                       // ensure null-termination
		printf("收到消息：%s\n", buf);                         // 显示所接收的字符串
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
