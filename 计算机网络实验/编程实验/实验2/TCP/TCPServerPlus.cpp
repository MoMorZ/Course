/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
#include "conio.h"

#define	WSVERS	MAKEWORD(2, 0)
#define BUF_LEN 2000
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library



void cat_message(char * buf, char *src) {
	strcat(buf, src);
	strcat(buf, "\n");
}

int main(int argc, char *argv[]) {
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock, ssock;		    /* master & slave sockets	      */
	WSADATA wsadata;
	const char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
	int	    alen;			        /* from-address length		        */
	char	*pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */
	char	buf[BUF_LEN + 1];   		    /* buffer for one line of text	*/
	char ip_buf[BUF_LEN + 1];
	char port_buf[BUF_LEN + 1];


	WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
	// ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(host to network��s-short 16λ)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�

	listen(msock, 5);                                   // ��������Ϊ5������������У����ѵ������������������еȴ�����
	printf("����������ɹ�\n\n");

	while (!_kbhit()) { 		                                   // ����Ƿ��а���,���û�������ѭ����ִ��
		alen = sizeof(struct sockaddr);                         // ȡ����ַ�ṹ�ĳ���
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // ����������������������������������������󲢽������ӣ����ظ����ӵ��׽��֣����򣬱���䱻����ֱ�����зǿա�fsin�����ͻ���IP��ַ�Ͷ˿ں�
		int rec_cc = recv(ssock, buf, BUF_LEN, 0);
		if (rec_cc == SOCKET_ERROR) {                        // ����������ر��׽���sock
			printf("����: %d.\n", GetLastError());
		} else if (rec_cc == 0) {                       // �Է������ر�
			printf("������ѹر�!\n");
		} else if (rec_cc > 0) {
			buf[rec_cc] = '\0';	                       // ensure null-termination
			printf("�յ���Ϣ��%s\n", buf);                         // ��ʾ�����յ��ַ���
		}

		(void)time(&now);                                      // ȡ��ϵͳʱ��
		pts = ctime(&now);                                      // ��ʱ��ת��Ϊ�ַ���
		pts[strlen(pts)-1] = '\0';
		strcat(buf, "\nʱ�䣺");
		cat_message(buf, pts);
		printf("�յ�ʱ�䣺%s", pts);                                      // ��ʾ���͵��ַ���

		//��ȡIP��ַ
		sprintf(ip_buf, "%u.%u.%u.%u",
		        fsin.sin_addr.S_un.S_un_b.s_b1,
		        fsin.sin_addr.S_un.S_un_b.s_b2,
		        fsin.sin_addr.S_un.S_un_b.s_b3,
		        fsin.sin_addr.S_un.S_un_b.s_b4);
		printf("�ͻ���IP��ַ��%s\n", ip_buf);
		strcat(buf, "�ͻ���IP��ַ��");
		cat_message(buf, ip_buf);

		//��ȡ�˿�
		sprintf(port_buf, "%hu", fsin.sin_port);
		printf("�ͻ��˶˿ںţ�%s\n", port_buf);
		strcat(buf, "�ͻ��˶˿ڣ�");
		cat_message(buf, port_buf);

		int cc = send(ssock, buf, strlen(buf), 0);
		if (cc == SOCKET_ERROR) {                         // ����������ر��׽���sock
			printf("����: %d.\n", GetLastError());
			closesocket(ssock);
			WSACleanup();
		} else if (cc == 0) {                       // �Է������ر�
			printf("������ѹر�!\n");
		} else if (cc > 0) {
			printf("���ͳɹ���\n\n");
		}

		(void)closesocket(ssock);                              // �ر������׽���
	}
	(void)closesocket(msock);                                 // �رռ����׽���
	WSACleanup();                                             // ж��winsock library
	printf("���س�������...");
	getchar();										// �ȴ����ⰴ��
	getchar();
}
