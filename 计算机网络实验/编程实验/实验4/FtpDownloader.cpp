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
char rec_buf[BUFLEN];//���ջ�����

int send_message(SOCKET sock, char *buf) {
	int send_cc = send(sock, buf, strlen(buf), 0);
	if (send_cc == SOCKET_ERROR) {                        // ����������ر��׽���sock
		printf("����ʧ��! ����: %d.\n", GetLastError());
		getchar();
		return 0;
	} else if (send_cc == 0) {                     // �Է������ر�
		printf("����ʧ��! ������ѹر�!\n");
		getchar();
		return 0;
	} else if (send_cc > 0) {
		printf("���ͳɹ�\n");
		return 1;
	}
	return 1;
}

unsigned __stdcall rec_message(void * p) {
	int rec_cc;
	SOCKET *pt = (SOCKET *)p;
	//char	buf[BUFLEN + 1];   		    /* buffer for one line of text	*/

	while (rec_cc = recv(*pt, rec_buf, BUFLEN, 0)) {
		if (rec_cc == SOCKET_ERROR) {                      // ����������ر��׽���sock
			//printf("����ʧ��! ����: %d.\n", GetLastError());
			//getchar();
			//return 0;
		} else if (rec_cc == 0) {                     // �Է������ر�
			printf("����ʧ��! ������ѹر�!\n");
			getchar();
			//return 0;
		} else if (rec_cc > 0) {
			rec_buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("�յ�����Ϣ��%s\n", rec_buf);            // ��ʾ�����յ��ַ���
		}
	}
	(void)closesocket(*pt);           //�ر������׽���
	return 0;
}

SOCKET dataConn(char *buf) {
	int ip1, ip2, ip3, ip4, port1, port2;
	char ip[20];
	int port;
	struct sockaddr_in sin;
	SOCKET data_sock;

	/*��ȡ��ַ�Ͷ˿�*/
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

	int ret = connect(data_sock, (struct sockaddr *)&sin, sizeof(sin)); // ���ӵ�������
	if (ret == 0) { //���ӳɹ�
		return data_sock;
	} else {
		printf("���ط���������ʧ��\n����%d\n", GetLastError());
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
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &rec_message, (void *)&sock, 0, NULL); //�������߳�

		//��¼
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "USER %s\r\n", argv[2]);
		send_message(sock, buf);
		sprintf(buf, "PASS %s\r\n", argv[3]);
		send_message(sock, buf);
		//��������
		sprintf(buf, "PASV\r\n");
		send_message(sock, buf);
		SOCKET data_sock;
		while ((data_sock = dataConn(rec_buf)) == NULL);
		//��ȡ�ļ�
		sprintf(buf, "RETR %s\r\n", argv[4]);
		send_message(sock, buf);
		saveFile(data_sock, argv[5]);

		closesocket(data_sock);
		CloseHandle(hThread);
	}
	closesocket(sock);                             // �رռ����׽���
	WSACleanup();                                  // ж��winsock library

	printf("������ɣ�");
	getchar();										// �ȴ����ⰴ��
	getchar();
	return 1;
}

int main(int argc, char* argv[]) {
	printf("FtpDownloader:\n");
	printf("Command: IPaddress User Password FileName DestFile:\n");
	if (argc != 6) {
		printf("��������!\n");
		return -1;
	}
	/*else {
		printf("�������£�%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
	}*/
	ConnectToServer(argv);
	return 0;
}