#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF_LEN 100
#define USER_NAME_LEN 20
#define EMAIL_LEN 80
#define TIME_BUF_LEN 30

typedef unsigned long DWORD;


int main() {
	char	pts[TIME_BUF_LEN];		        /* pointer to time string	        */
	time_t	now;			        /* current time			            */

	struct Person {
		char username[USER_NAME_LEN];          /*  �û���      */
		int level;                  /*  ����        */
		char email[EMAIL_LEN];             /*  email��ַ   */
		DWORD sendtime;
		time_t regtime;
	};

	(void)time(&now);                                       // ȡ��ϵͳʱ��
	ctime_s(pts, TIME_BUF_LEN, &now);                          // ��ʱ��ת��Ϊ�ַ���
	printf(pts);

	struct Person personSent;
	char inputBuf[100];
	int inputNumber;

	FILE *pFile;
	if ((pFile = fopen("Persons.txt", "wb")) == NULL) {
		printf("can not open the file");
		getchar();
		exit(0);
	}


	while (1) {
		printf("username: ");
		scanf_s("%s", inputBuf, USER_NAME_LEN);
		if (strcmp(inputBuf, "exit") == 0) {
			printf("\nInput has been terminated\n\n");
			break;
		}


		strcpy_s(personSent.username, inputBuf);
		printf("level: ");
		scanf_s("%d", &inputNumber, sizeof(int));
		personSent.level = inputNumber;
		printf("email: ");
		scanf_s("%s", inputBuf, EMAIL_LEN);
		strcpy_s(personSent.email, inputBuf);
		personSent.sendtime = (DWORD)now;
		personSent.regtime = now;
		//memcpy(buf, &personSent, sizeof(Person));

		if (fwrite(&personSent, sizeof(Person), 1, pFile) != 1)
			printf("file write error\n");
	}
	fclose(pFile);

	FILE * fp;
	if ((fp = fopen("Persons.txt", "rb")) == NULL) {
		printf("can not open the file");
		getchar();
		exit(0);
	}


	struct Person personRecv;
	char regtime[TIME_BUF_LEN];
	char sendtime[TIME_BUF_LEN];
	while (fread(&personRecv, sizeof(Person), 1, fp) == 1) { //����������ݣ�����ʾ�������˳�
		//memcpy(&personRecv, buf, sizeof(Person));
		printf("�û����� %s\r\n", personRecv.username);
		printf("����%d\r\n", personRecv.level);
		printf("Email��ַ��%s\r\n", personRecv.email);
		time_t t1 = (time_t)personRecv.sendtime;
		ctime_s(sendtime, TIME_BUF_LEN, &t1);               // ��ʱ��ת��Ϊ�ַ���
		printf("����ʱ�䣺%s", sendtime);
		ctime_s(regtime, TIME_BUF_LEN, &personRecv.regtime);               // ��ʱ��ת��Ϊ�ַ���
		printf("ע��ʱ�䣺%s", regtime);
	}


	printf("\r\nPress any key to continue...");
	getchar();
	getchar();
	return 0;
}