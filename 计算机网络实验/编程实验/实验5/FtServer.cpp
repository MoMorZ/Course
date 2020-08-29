/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "conio.h"
#include <io.h>
#define	BUFLEN		2000                  // ��������С
#define	WSVERS	MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library

void getValidName(char *);

int main()
/* argc: �����в��������� ���磺C:\> TCPServer 8080
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


    WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
    msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
    // ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
    sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
    sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
    sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(host to network��s-short 16λ)
    bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�
    listen(msock, 5);                                   // ��������Ϊ5������������У����ѵ������������������еȴ�����

    printf("���ӳɹ�!\n");
    printf("��������ļ���:\n");
    scanf("%s", filepath);
    if (filepath[strlen(filepath) - 1] != '\\') {
        filepath[strlen(filepath) + 1] = '\0';
        filepath[strlen(filepath)] = '\\';
    }

    while (!_kbhit()) { 		                                   // ����Ƿ��а���,���û�������ѭ����ִ��
        alen = sizeof(struct sockaddr);                         // ȡ����ַ�ṹ�ĳ���
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // ����������������������������������������󲢽������ӣ����ظ����ӵ��׽��֣����򣬱���䱻����ֱ�����зǿա�fsin�����ͻ���IP��ַ�Ͷ˿ں�
        memset(filename, 0, sizeof(filename));
        strcpy(fullpath, filepath);
        cc = recv(ssock, filename, 100, 0);
        filename[strlen(filename)] = '\0';
        printf("���ڽ���: %s\n", filename);
        if (cc == 0) {
            printf("�յ��˳�����!\n");
            break;
        }
        strcat(fullpath, filename);
        getValidName(fullpath);
        fp = fopen(fullpath, "wb");
        while ((filelen = recv(ssock, buf, BUFLEN, 0)) > 0)
            fwrite(buf, sizeof(char), filelen, fp);

        fclose(fp);
        printf("�������\n");
        strcpy(buf, "���������ճɹ�!\n");
        send(ssock, buf, strlen(buf) + 1, 0);
        shutdown(ssock, SD_SEND);
        (void)closesocket(ssock);                              // �ر������׽���
    }
    (void)closesocket(msock);                                 // �رռ����׽���
    WSACleanup();                                             // ж��winsock library
    printf("�������ѹر�\n");
    printf("���س�������...");
    getchar();										// �ȴ����ⰴ��
    getchar();
}

void getValidName(char *filepath) {
    char tmp[100], head[50], tail[50], strnum[50];
    int pos1 = strrchr(filepath, '(') - filepath;
    int pos2 = strrchr(filepath, '.') - filepath, num = 0;
    if (strrchr(filepath, '(')) {
        strncpy(head, filepath, pos1);//.ǰ��Ĳ���
        head[pos1] = '\0';
    } else {
        strncpy(head, filepath, pos2);//.ǰ��Ĳ���
        head[pos2] = '\0';
    }
    strcpy(tail, filepath + pos2);//.����Ĳ���
    strcpy(tmp, filepath);
    if (strrchr(filepath, '('))
        sscanf(tmp + pos1, "%d", &num);
    while (_access(tmp, 0) == 0) {
        ++num;
        sprintf(tmp, "%s(%d)%s", head, num, tail);
    }
    strcpy(filepath, tmp);
}