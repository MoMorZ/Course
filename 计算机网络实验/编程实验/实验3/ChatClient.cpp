/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include "conio.h"

#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary
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
	if (send_cc == SOCKET_ERROR) {                        // ����������ر��׽���sock
		printf("����ʧ��! ����: %d.\n", GetLastError());
		getchar();
		return 0;
	} else if (send_cc == 0) {                         // �Է������ر�
		printf("����ʧ��! ������ѹر�!\n");
		getchar();
		return 0;
	} else if (send_cc > 0) {
		printf("���ͳɹ�\n");
		return 1;
	}
}


unsigned __stdcall rec_message(void * p) {
	int rec_cc;
	SOCKET *pt = (SOCKET *)p;
	char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/

	while (rec_cc = recv(*pt, buf, BUFLEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                      // ����������ر��׽���sock
			printf("����ʧ��! ����: %d.\n", GetLastError());
			getchar();
			return 0;
		} else if (rec_cc == 0) {                         // �Է������ر�
			printf("����ʧ��! ������ѹر�!\n");
			getchar();
			return 0;
		} else if (rec_cc > 0) {
			buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("�յ�����Ϣ��%s\n", buf);            // ��ʾ�����յ��ַ���
		}
	}
	(void)closesocket(*pt);           //�ر������׽���
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
	WSAStartup(WSVERS, &wsadata);						  //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							  // ��������ַ��
	sin.sin_addr.s_addr = inet_addr(host);                // ���÷�����IP��ַ(32λ)
	sin.sin_port = htons((u_short)atoi(service));         // ���÷������˿ں�

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��

	int ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // ���ӵ�������
	if (ret == SOCKET_ERROR) {
		printf("����������ʧ��\n");
		getchar();
	} else {
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &rec_message, (void *)&sock, 0, NULL); //�������߳�
		while (1) {
			printf("������Ҫ���͵���Ϣ��\n");
			scanf_s("%s", buf, BUFLEN);
			if (strcmp(buf, "exit") == 0) {
				CloseHandle(hThread);
				printf("�ͻ����˳�\n");
				return 0;
			}
			if (0 == send_message(buf)) {
				CloseHandle(hThread);
				return 0;
			}
		}
	}



	closesocket(sock);                             // �رռ����׽���
	WSACleanup();                                  // ж��winsock library

	printf("���س�������...");
	getchar();										// �ȴ����ⰴ��
	getchar();
	return 1;
}


int main(int argc, char *argv[]) {
	printf("�ͻ��˳ɹ��������˳�������exit\n");
	while (ConnectToServer() != 0);
	printf("�ͻ��������˳������س�ȷ��\n");
	getchar();
	return 0;
}
