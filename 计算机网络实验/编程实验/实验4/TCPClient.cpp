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

#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary


unsigned __stdcall send_message(void * p) {
	SOCKET *pt = (SOCKET *)p;
	char send_buf[BUFLEN + 1]; //���ͻ�����
	while (1) {
		fgets(send_buf, BUFLEN, stdin);
		int size = strlen(send_buf) - 1;
		send_buf[size] = '\r';
		send_buf[size + 1] = '\n';
		int send_cc = send(*pt, send_buf, size + 2, 0);
		if (send_cc == SOCKET_ERROR) {                        // ����������ر��׽���sock
			printf("����ʧ��! ����: %d.\n", GetLastError());
		} else if (send_cc == 0) {                   // �Է������ر�
			printf("����ʧ��! ������ѹر�!\n");
		} else if (send_cc > 0) {
			printf("���ͳɹ�\n");
		}
	}
	return 1;
}

unsigned __stdcall rec_message(void * p) {
	int rec_cc;
	SOCKET *pt = (SOCKET *)p;
	char rec_buf[BUFLEN + 1]; //���ջ�����

	while (rec_cc = recv(*pt, rec_buf, BUFLEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                      // ����������ر��׽���sock
			printf("����ʧ��! ����: %d.\n", GetLastError());
		} else if (rec_cc == 0) {                   // �Է������ر�
			printf("����ʧ��! ������ѹر�!\n");
		} else if (rec_cc > 0) {
			rec_buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("%s", rec_buf);            // ��ʾ�����յ��ַ���
		}
	}
	(void)closesocket(*pt);           //�ر������׽���
	return 0;
}

int ConnectToServer(char *argv[]) {
	const char	*host = argv[1];	    /* server IP to connect         */
	const u_short service_port = atoi(argv[2]);  	    /* server port to connect       */
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	DWORD recExit, sendExit;
	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							  // ��������ַ��
	sin.sin_addr.s_addr = inet_addr(host);                // ���÷�����IP��ַ(32λ)
	sin.sin_port = htons(service_port);         // ���÷������˿ں�

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��

	int ret = connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // ���ӵ�������
	if (ret == SOCKET_ERROR) {
		printf("����������ʧ��\n");
		getchar();
	} else {
		HANDLE rechThread = (HANDLE)_beginthreadex(NULL, 0, &rec_message, (void *)&sock, 0, NULL); //�������߳�
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
	closesocket(sock);                             // �رռ����׽���
	WSACleanup();                                  // ж��winsock library

	printf("�˳���");
	getchar();										// �ȴ����ⰴ��
	return 1;
}

int main(int argc, char* argv[]) {
	printf("TCPClient:\n");
	printf("Command: IPaddress IP Port\n");
	if (argc != 3) {
		printf("��������!\n");
		return -1;
	} else {
		printf("�������£�\n%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
	}
	ConnectToServer(argv);
	return 0;
}