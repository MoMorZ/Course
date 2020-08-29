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
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library
#define MAX_CON 100

SOCKET ssock[MAX_CON];		//�׽�������
HANDLE hThread[MAX_CON];	 //���߳���
CRITICAL_SECTION cs;		 // �ٽ���

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
		if (send_cc == SOCKET_ERROR) {                         // ����������ر��׽���sock
			printf("����: %d.\n", GetLastError());
			EnterCriticalSection(&cs);				// �ȴ������ٽ���
			closesocket(ssock[i]);
			ssock[i] = NULL;								//ͬһʱ��ֻ����һ���߳��޸��׽�����
			LeaveCriticalSection(&cs);			    // �뿪�ٽ���
		} else if (send_cc == 0) {                   // �Է������ر�
			printf("������ѹر�!\n");
		} else if (send_cc > 0) {
			printf("���ͳɹ���\n");
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
		if (rec_cc == SOCKET_ERROR) {                        // ����������ر��׽���sock
			printf("����: %d.\n", GetLastError());
			getchar();
			break;
		} else if (rec_cc == 0) {                   // �Է������ر�
			printf("������ѹر�!\n");
			getchar();
			break;
		} else if (rec_cc > 0) {
			buf[rec_cc] = '\0';	                       // ensure null-termination
			(void)time(&now);                                      // ȡ��ϵͳʱ��
			pts = ctime(&now);                                      // ��ʱ��ת��Ϊ�ַ���
			pts[strlen(pts) - 1] = '\0';
			strcat(buf, "\nʱ�䣺");
			cat_message(buf, pts);
			//printf("�յ�ʱ�䣺%s", pts);                                      // ��ʾ���͵��ַ���
			//��ȡIP��ַ
			//printf("�ͻ���IP��ַ��%s\n",pt->IP);
			strcat(buf, "\n�ͻ���IP��ַ��");
			cat_message(buf, pt->IP);
			//��ȡ�˿�
			//printf("�ͻ��˶˿ںţ�%s\n", pt->port);
			strcat(buf, "\n�ͻ��˶˿ڣ�");
			cat_message(buf, pt->port);
			printf("�յ���Ϣ��%s\n", buf);                         // ��ʾ�����յ��ַ���
			send_to_all(buf);
		}
	}


	EnterCriticalSection(&cs);				// �ȴ������ٽ���
	(void)closesocket(ssock[pt->pos]);        // �ر������׽���
	ssock[pt->pos] = NULL;                   //����
	CloseHandle(hThread[pt->pos]);           //�ر��߳�
	LeaveCriticalSection(&cs);				// �뿪�ٽ���
	return 0;
}

int main(int argc, char *argv[]) {
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock;		    /* master & slave sockets	      */
	WSADATA wsadata;
	const char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
	int	    alen = sizeof(struct sockaddr);			        /* from-address length		        */


	WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
	memset(ssock, 0, sizeof(ssock));
	memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	memset(&param, 0, sizeof(param));
	InitializeCriticalSection(&cs);				        // ��ʼ���ٽ���
	sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(host to network��s-short 16λ)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�

	listen(msock, MAX_CON);                                   // ��������Ϊ5������������У����ѵ������������������еȴ�����
	printf("����������ɹ�\n");

	while (!_kbhit()) { 		                                   // ����Ƿ��а���,���û�������ѭ����ִ��
		int pos = 0;
		while (pos < MAX_CON && ssock[pos] != NULL) pos++;
		if (pos == MAX_CON) {
			printf("����������\n");
		} else {
			param[pos].pos = pos;			//����׽���λ��
			ssock[pos] = accept(msock, (struct sockaddr *)&fsin, &alen);		//�������󲢽�������
			strcpy(param[pos].IP, inet_ntoa(fsin.sin_addr));			//��ȡ�ͻ���IP
			sprintf(param[pos].port, "%hu", fsin.sin_port);      //��ÿͻ��˶˿ں�
			hThread[pos] = (HANDLE)_beginthreadex(NULL, 0, &transmit, (void *)&param[pos], 0, NULL);	//�������߳�
		}
	}
	(void)closesocket(msock);                                 // �رռ����׽���
	WSACleanup();                                             // ж��winsock library
	DeleteCriticalSection(&cs);		                          // ɾ���ٽ���
	for (int i = 0; i < MAX_CON; i++) {
		if (ssock[i] != NULL)
			CloseHandle(hThread[i]);	//�ͷ��߳�
	}
	printf("���س�������...");
	getchar();										// �ȴ����ⰴ��
	getchar();
}