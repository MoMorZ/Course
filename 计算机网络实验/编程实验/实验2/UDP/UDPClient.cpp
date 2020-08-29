/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary

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
	WSAStartup(WSVERS, &wsadata);       /* ����ĳ�汾Socket��DLL        */

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons((u_short)atoi(service));    //atoi����asciiת��Ϊint. htons��������(host)ת��Ϊ������(network), s--short
	sin.sin_addr.s_addr = inet_addr(host);           //���hostΪ��������Ҫ���ú���gethostbyname������ת��ΪIP��ַ

	sock = socket(PF_INET,  SOCK_DGRAM, IPPROTO_UDP);	  //�����׽��֣�������������Э���(family)�����׽��֣�UDPЭ��
	printf("������Ҫ���͵���Ϣ��");
	scanf_s("%s", buf, BUFLEN);
	if (strcmp(buf, "exit") == 0) {
		printf("�ͻ����˳�\n");
		return 0;
	}

	int send_cc = sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&sin, sizeof(sin));
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
	}


	int rec_cc = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&sin, &sin_size);
	if (rec_cc == SOCKET_ERROR) {                        // ����������ر��׽���sock
		printf("����ʧ��! ����: %d.\n", GetLastError());
	} else if (rec_cc == 0) {                     // �Է������ر�
		printf("����ʧ��! ������ѹر�!\n");
	} else if (rec_cc > 0) {
		buf[rec_cc] = '\0';	                       // ensure null-termination
		printf("�յ���Ϣ��%s\n", buf);                         // ��ʾ�����յ��ַ���
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
