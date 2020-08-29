/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "conio.h"
#include <io.h>
#define	BUFLEN		2000                  // 缓冲区大小
#define	WSVERS	MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library

void getValidName(char *);

int main()
/* argc: 命令行参数个数， 例如：C:\> TCPServer 8080
					 argc=2 argv[0]="TCPServer",argv[1]="8080" */
{
    struct	sockaddr_in fsin;	    /* the from address of a client	  */
    SOCKET	msock, ssock;		    /* master & slave sockets	      */
    WSADATA wsadata;
    const char	*service = "50500";
    struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
    int	    alen;			        /* from-address length		        */
    char buf[BUFLEN + 1], filepath[100], filename[100], fullpath[100];
    int cc, filelen;
    FILE *fp;


    WSAStartup(WSVERS, &wsadata);						// 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
    msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
    // 返回：要监听套接字的描述符或INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						// 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							// 因特网地址簇(INET-Internet)
    sin.sin_addr.s_addr = INADDR_ANY;					// 监听所有(接口的)IP地址。
    sin.sin_port = htons((u_short)atoi(service));		// 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(host to network，s-short 16位)
    bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // 绑定监听的IP地址和端口号
    listen(msock, 5);                                   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理。

    printf("连接成功!\n");
    printf("输入接收文件夹:\n");
    scanf("%s", filepath);
    if (filepath[strlen(filepath) - 1] != '\\') {
        filepath[strlen(filepath) + 1] = '\0';
        filepath[strlen(filepath)] = '\\';
    }

    while (!_kbhit()) { 		                                   // 检测是否有按键,如果没有则进入循环体执行
        alen = sizeof(struct sockaddr);                         // 取到地址结构的长度
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // 如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号
        memset(filename, 0, sizeof(filename));
        strcpy(fullpath, filepath);
        cc = recv(ssock, filename, 100, 0);
        filename[strlen(filename)] = '\0';
        printf("正在接收: %s\n", filename);
        if (cc == 0) {
            printf("收到退出命令!\n");
            break;
        }
        strcat(fullpath, filename);
        getValidName(fullpath);
        fp = fopen(fullpath, "wb");
        while ((filelen = recv(ssock, buf, BUFLEN, 0)) > 0)
            fwrite(buf, sizeof(char), filelen, fp);

        fclose(fp);
        printf("接收完毕\n");
        strcpy(buf, "服务器接收成功!\n");
        send(ssock, buf, strlen(buf) + 1, 0);
        shutdown(ssock, SD_SEND);
        (void)closesocket(ssock);                              // 关闭连接套接字
    }
    (void)closesocket(msock);                                 // 关闭监听套接字
    WSACleanup();                                             // 卸载winsock library
    printf("服务器已关闭\n");
    printf("按回车键继续...");
    getchar();										// 等待任意按键
    getchar();
}

void getValidName(char *filepath) {
    char tmp[100], head[50], tail[50], strnum[50];
    int pos1 = strrchr(filepath, '(') - filepath;
    int pos2 = strrchr(filepath, '.') - filepath, num = 0;
    if (strrchr(filepath, '(')) {
        strncpy(head, filepath, pos1);//.前面的部分
        head[pos1] = '\0';
    } else {
        strncpy(head, filepath, pos2);//.前面的部分
        head[pos2] = '\0';
    }
    strcpy(tail, filepath + pos2);//.后面的部分
    strcpy(tmp, filepath);
    if (strrchr(filepath, '('))
        sscanf(tmp + pos1, "%d", &num);
    while (_access(tmp, 0) == 0) {
        ++num;
        sprintf(tmp, "%s(%d)%s", head, num, tail);
    }
    strcpy(filepath, tmp);
}