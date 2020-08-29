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
char rec_buf[BUFLEN];//接收缓冲区

int send_message(SOCKET sock, char *buf) {
	int send_cc = send(sock, buf, strlen(buf), 0);
	if (send_cc == SOCKET_ERROR) {                        // 出错。其后必须关闭套接字sock
		printf("发送失败! 错误: %d.\n", GetLastError());
		getchar();
		return 0;
	} else if (send_cc == 0) {                     // 对方正常关闭
		printf("发送失败! 服务端已关闭!\n");
		getchar();
		return 0;
	} else if (send_cc > 0) {
		printf("发送成功\n");
		return 1;
	}
	return 1;
}

unsigned __stdcall rec_message(void * p) {
	int rec_cc;
	SOCKET *pt = (SOCKET *)p;
	//char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/

	while (rec_cc = recv(*pt, rec_buf, BUFLEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                      // 出错。其后必须关闭套接字sock
			//printf("接收失败! 错误: %d.\n", GetLastError());
			//getchar();
			//return 0;
		} else if (rec_cc == 0) {                     // 对方正常关闭
			printf("接收失败! 服务端已关闭!\n");
			getchar();
			//return 0;
		} else if (rec_cc > 0) {
			rec_buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("收到的消息：%s\n", rec_buf);            // 显示所接收的字符串
		}
	}
	(void)closesocket(*pt);           //关闭连接套接字
	return 0;
}

SOCKET dataConn(char *buf) {
	int ip1, ip2, ip3, ip4, port1, port2;
	char ip[20];
	int port;
	struct sockaddr_in sin;
	SOCKET data_sock;

	/*获取地址和端口*/
	char *st = strchr(buf, '(');
	if (st == NULL) return NULL;
	sscanf(st + 1, "%d,%d,%d,%d,%d,%d", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
	sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
	port = 256 * port1 + port2;

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_port = htons(port);
	data_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	int ret = connect(data_sock, (struct sockaddr *)&sin, sizeof(sin)); // 连接到服务器
	if (ret == 0) { //连接成功
		return data_sock;
	} else {
		printf("下载服务器连接失败\n错误：%d\n", GetLastError());
		return NULL;
	}
}

int saveFile(SOCKET sock, char * fileName) {
	FILE* destfile = fopen(fileName, "wb");
	int rec_cc;
	char buf[BUFLEN];
	while ((rec_cc = recv(sock, buf, BUFLEN, 0)) > 0) {
		fwrite(buf, 1, rec_cc, destfile);
	}
	fclose(destfile);
	return 1;
}

int ConnectToServer(char *argv[]) {
	const char	*host = argv[1];	    /* server IP to connect         */
	const u_short service_port = 21;  	    /* server port to connect       */
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/

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
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &rec_message, (void *)&sock, 0, NULL); //创建新线程

		//登录
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "USER %s\r\n", argv[2]);
		send_message(sock, buf);
		sprintf(buf, "PASS %s\r\n", argv[3]);
		send_message(sock, buf);
		//被动连接
		sprintf(buf, "PASV\r\n");
		send_message(sock, buf);
		SOCKET data_sock;
		while ((data_sock = dataConn(rec_buf)) == NULL);
		//获取文件
		sprintf(buf, "RETR %s\r\n", argv[4]);
		send_message(sock, buf);
		saveFile(data_sock, argv[5]);

		closesocket(data_sock);
		CloseHandle(hThread);
	}
	closesocket(sock);                             // 关闭监听套接字
	WSACleanup();                                  // 卸载winsock library

	printf("传输完成！");
	getchar();										// 等待任意按键
	getchar();
	return 1;
}

int main(int argc, char* argv[]) {
	printf("FtpDownloader:\n");
	printf("Command: IPaddress User Password FileName DestFile:\n");
	if (argc != 6) {
		printf("参数错误!\n");
		return -1;
	}
	/*else {
		printf("参数如下：%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
	}*/
	ConnectToServer(argv);
	return 0;
}