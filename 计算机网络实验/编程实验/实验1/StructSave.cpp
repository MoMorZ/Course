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
		char username[USER_NAME_LEN];          /*  用户名      */
		int level;                  /*  级别        */
		char email[EMAIL_LEN];             /*  email地址   */
		DWORD sendtime;
		time_t regtime;
	};

	(void)time(&now);                                       // 取得系统时间
	ctime_s(pts, TIME_BUF_LEN, &now);                          // 把时间转换为字符串
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
	while (fread(&personRecv, sizeof(Person), 1, fp) == 1) { //如果读到数据，就显示；否则退出
		//memcpy(&personRecv, buf, sizeof(Person));
		printf("用户名： %s\r\n", personRecv.username);
		printf("级别：%d\r\n", personRecv.level);
		printf("Email地址：%s\r\n", personRecv.email);
		time_t t1 = (time_t)personRecv.sendtime;
		ctime_s(sendtime, TIME_BUF_LEN, &t1);               // 把时间转换为字符串
		printf("发送时间：%s", sendtime);
		ctime_s(regtime, TIME_BUF_LEN, &personRecv.regtime);               // 把时间转换为字符串
		printf("注册时间：%s", regtime);
	}


	printf("\r\nPress any key to continue...");
	getchar();
	getchar();
	return 0;
}