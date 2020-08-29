__asm__(".code16gcc\r\n");
typedef unsigned int size_t;
#define NULL 0

extern void FuncPrintC();
extern void FuncPrintS();
extern void FuncPrintSL();
extern void FuncPrintSLRC();
extern void FuncClear();
extern void FuncGetChar();
extern void FuncReadFAT();
extern void FuncRunP();
extern void FuncReadFile();
extern void FuncSetClock();
extern void FuncIOout();
extern int FuncIOin(int port);
extern void FuncSetInt3X();
extern void FuncRunInt33();
extern void FuncRunInt34();
extern void FuncRunInt35();
extern void FuncRunInt36();
extern void FuncGetDate();
extern void FuncGetTime();


const int PIC_OCW2 = 0x20;
const int PORT_KEYDAT = 0X60;

//全局变量
const char newline[] = "\r\n";
char buffer[512];
int BuffLen;
int input;
int RowPos = 1;
int diff;
int InfoCount = 0;
char FileName[8];
int SecNo;
char Command[50];
char FAT[512];
char temp[8];
int NumFile = 5;

int century;
int year;
int month;
int day;
int hour;
int minute;
int second;

//局部可能用到的
int i = 0, j = 0, len = 0;

//函数声明

char * strcat(char * dest, const char * src);
void*memset(void *s, int c, size_t n) ;
void * memmove(void*, const void*, size_t);
char * fstrcpy(char* dest, const char* src);
char * fstrncpy(char* dest, const char* src, int len);
int strcmp(const char * d, const char * s);
size_t strlen(const char * s);
void runProg(int);
char * getCom();
void runCom(char *);
int getFile(char *);
void readFile(int SecNo);
void mycmain();
void showFAT();
void intHandleKeyB();
void welcome();
int  BCDToDEC(int);
void intToChar(int, char*);
void printDate();
void printTime();

//前8字节文件名，9、10文件所在扇区，11、12文件大小，13、14文件创建时间，15，16文件类型

const char MessHello[40] = "Hello,OS!";


char * strcat(char * dest, const char * src) {
	char *tmp = dest;
	while (*dest) dest++;
	while ((*dest++ = *src++) != '\0');
	return tmp;
}

void *memset(void *s, int c, size_t n) {
	const unsigned char uc = c;
	unsigned char *su;
	for (su = s; 0 < n; ++su, --n)
		*su = uc;
	return s;
}

void* memmove(void* dst, const void* src, size_t count) {
	//assert(NULL != src && NULL != dst);
	char* tmpdst = (char*)dst;
	char* tmpsrc = (char*)src;

	if (tmpdst <= tmpsrc || tmpdst >= tmpsrc + count) {
		while (count--) {
			*tmpdst++ = *tmpsrc++;
		}
	} else {
		tmpdst = tmpdst + count - 1;
		tmpsrc = tmpsrc + count - 1;
		while (count--) {
			*tmpdst-- = *tmpsrc--;
		}
	}
	return dst;
}

char* fstrcpy(char* dest, const char* src) {
	char* temp = dest;
	while ((*temp++ = *src++ ) != 0);
	return dest;
}

char* fstrncpy(char* dest, const char* src, int len) {
	if (dest == NULL || src == NULL)
		return NULL;
	char* temp = dest;
	int i = 0;
	while (i++ < len  && (*temp++ = *src++) != '\0');
	if (*(temp) != '\0')
		*temp = '\0';
	return dest;
}

int strcmp(const char * d, const char * s) {
	while (*s != '\0' && *d != '\0') {
		if (*d == *s)
			++s, ++d;
		else
			return *d - *s;
	}
	return *d - *s;
}

size_t strlen(const char * s) {
	size_t cnt = 0;
	while (*s++ != '\0')
		++cnt;
	return cnt;
}

const char RunTips1[] = "Trying to run command...";
const char RunTips3[] = "Run successfully!";

void runCom(char *Command) {
	FuncPrintSL(RunTips1, strlen(RunTips1));
	memset(FileName, 0, 8);
	i = 0, j = 0;
	len = strlen(Command);
	FuncPrintSL("runCCom:", 8);
	FuncPrintSL(Command, strlen(Command));

	while (i < len) {
		//FuncPrintSL("TEST", 4);
		j = i;
		//FuncPrintC(i + 48);
		//FuncPrintSL("", 0);
		while (Command[j] != ' ' && Command[j] != '\0') {
			j++;
		}
		//FuncPrintC(j + 48);
		//FuncPrintSL("", 0);
		fstrncpy(FileName, Command + i, j - i);
		FuncPrintSL("runCFileName:", 13);
		FuncPrintSL(FileName, strlen(FileName));
		int SecNo = getFile(FileName);
		runProg(SecNo);
		i = j + 1;
	}
	FuncPrintSL(RunTips3, strlen(RunTips3));
	return;
}

const char FileTips1[] = "Tring to get file...";

int getFile(char *FileName) {
	int SecNo = 0;
	FuncPrintSL(FileTips1, strlen(FileTips1));
	FuncReadFAT(FAT);
	NumFile = 5;
	InfoCount = 0;
	do {
		fstrncpy(temp, FAT + InfoCount, 8);
		SecNo = FAT[InfoCount + 9];//取得扇区号
		//FuncPrintSL("temp:", 5);
		//FuncPrintSL(temp, 8);
		diff = strcmp(temp, FileName);
		InfoCount += 16;
	} while ( (InfoCount < (NumFile * 16)) && (diff != 0));
	FuncPrintSL("getF_SecNo:", 6 + 5);
	FuncPrintC(SecNo + 48);
	FuncPrintSL("", 0);
	return SecNo;
}

const char ReadTips1[] = "Read file successfully!";

void readFile(int SecNo) {
	FuncReadFile(buffer, SecNo);
	FuncPrintSL(ReadTips1, strlen(ReadTips1));
	return;
}



const char ProgTips1[] = "Tring to load progress...";
const char ProgTips2[] = "Load progress successfully!";

void runProg(int SecNo) {
	FuncPrintSL(ProgTips1, strlen(ProgTips1));
	FuncPrintSL("SecNo:", 6);
	FuncPrintC(SecNo + 48);
	FuncPrintSL("", 0);
	FuncRunP(SecNo);
	FuncPrintSL(ProgTips2, strlen(ProgTips2));
	return;
}


const char Comtips1[] = "Please input your command:";

char * getCom() {
	/*读取批处理命令部分*/
	BuffLen = strlen(Comtips1);
	FuncPrintSL(Comtips1, BuffLen);
	//char Command[50];
	memset(Command, 0, 50);
	i = 0;
	while (1) {
		FuncGetChar();
		//FuncPrintC(input);
		if (input == '\n' || input == '\r') break;
		//FuncPrintC(input);
		Command[i++] = input;
	}
	Command[i + 1] = '\0';
	FuncPrintSL("getCCommand:", 12);
	FuncPrintSL(Command, strlen(Command));
	return Command;
}

const char ScriptTips1[] = "Loading Script...";
const char ScriptTips2[] = "Run Script successfully!";
void runScript() {
	FuncPrintSL(ScriptTips1, strlen(ScriptTips1));
	memset(FileName, 0, 8);
	i = 0, j = 0;
	fstrcpy(FileName, Command);
	//FuncPrintSL("test1:", 6);
	//FuncPrintSL(FileName, strlen(FileName));
	memset(buffer, 0, 512);
	memset(Command, 0, 50);
	int SecNo = getFile(FileName);
	readFile(SecNo);
	fstrncpy(Command, buffer, 50);
//	FuncPrintSL("test2:", 6);
//	FuncPrintSL(buffer, 512);
//	FuncPrintSL(Command, strlen(Command));
	runCom(Command);
	return;
}

const char FATtips1[] = "FAT:";
const char FATtips2[] = "Name    No";


void showFAT() {
	FuncPrintSL(FATtips1, strlen(FATtips1));
	FuncPrintSL(FATtips2, strlen(FATtips2));
	FuncReadFAT(FAT);
	//FuncPrintSL(FAT, 512);
	i = 0;
	NumFile = 5;

	while (i < 16 * NumFile) {
		FuncPrintS(FAT + i, 8);
		input = FAT[i + 9];
		if (input < 10) {
			input += 48;
		} else if (input >= 10 && input < 16) {
			input += 55;
		}
		FuncPrintC(input);
		i += 16;
	}
	FuncPrintSL("", 0);
	return;
}

const char intTips1[] = "Please input number to choose the INT function";
const char intTips2[] = "1.Show the ID number";
const char intTips3[] = "2.Show the name";
const char intTips4[] = "3.Show the date";
const char intTips5[] = "4.Show the time";
const char intTips6[] = "0.Exit";

void showIntMenu() {
	FuncPrintSL(intTips1, strlen(intTips1));
	FuncPrintSL(intTips2, strlen(intTips2));
	FuncPrintSL(intTips3, strlen(intTips3));
	FuncPrintSL(intTips4, strlen(intTips4));
	FuncPrintSL(intTips5, strlen(intTips5));
	FuncPrintSL(intTips6, strlen(intTips6));
}

void runInt() {
	FuncClear();
	showIntMenu();
	while (1) {
		FuncGetChar();
		FuncPrintSL("", 0);
		switch (input) {
		case '1':
			FuncRunInt33();
			break;
		case '2':
			FuncRunInt34();
			break;
		case '3':
			FuncRunInt35();
			break;
		case '4':
			FuncRunInt36();
			break;
		case '0':
			FuncClear();
			return;
		}
	}
}

int BCDToDEC(int n) {
	return n / 16 * 10 + n % 16;
}

void intToChar(int n, char *s) {
	s[0] = n / 10 + 48;
	s[1] = n % 10 + 48;
	s[2] = '\0';
}

void printDate() {
	FuncGetDate();
	char buf[3];
	char date[15];
	memset(date, 0, strlen(date));
	intToChar(BCDToDEC(century), buf);
	strcat(date, buf);
	intToChar(BCDToDEC(year), buf);
	strcat(date, buf);
	strcat(date, "/");
	intToChar(BCDToDEC(month), buf);
	strcat(date, buf);
	strcat(date, "/");
	intToChar(BCDToDEC(day), buf);
	strcat(date, buf);
	FuncPrintSLRC(date, strlen(date), 7, 60);
	return;
}

void printTime() {
	FuncGetTime();
	char buf[3];
	char time[15];
	memset(time, 0, strlen(time));
	intToChar(BCDToDEC(hour), buf);
	strcat(time, buf);
	strcat(time, ":");
	intToChar(BCDToDEC(minute), buf);
	strcat(time, buf);
	strcat(time, ":");
	intToChar(BCDToDEC(second), buf);
	strcat(time, buf);
	FuncPrintSLRC(time, strlen(time), 8, 60);
	return;
}


const char mainTips1[] = "Please input number to choose the function";
const char mainTips2[] = "1.Show the FAT";
const char mainTips3[] = "2.Run Command";
const char mainTips4[] = "3.Run Script";
const char mainTips5[] = "0.Exit";
const char mainTips6[] = "END!";
const char mainTips7[] = "4.Clear Screen";
const char mainTips8[] = "5.Call Int";


void welcome() {
	FuncPrintSL(MessHello, strlen(MessHello));
	FuncPrintSL(mainTips1, strlen(mainTips1));
	FuncPrintSL(mainTips2, strlen(mainTips2));
	FuncPrintSL(mainTips3, strlen(mainTips3));
	FuncPrintSL(mainTips4, strlen(mainTips4));
	FuncPrintSL(mainTips7, strlen(mainTips7));
	FuncPrintSL(mainTips8, strlen(mainTips8));
	FuncPrintSL(mainTips5, strlen(mainTips5));
	return;
}


void intHandleKeyB() {
	FuncIOout(PIC_OCW2, 0x21);
	int ch = FuncIOin(PORT_KEYDAT);
	FuncPrintC(ch);
	return;
}

void delay() {
	int max = 1000;
	for (i = 0; i < max; i++) {
		for (j = 0; j < max; j++) {

		}
	}
}

void mycmain() {
	FuncSetClock();//无敌风火轮
	FuncSetInt3X();
	/*while (1) {
		intHandleKeyB();//键盘中断
		delay();
	}*/



	while (1) {
		welcome();
		FuncGetChar();
		FuncPrintSL("", 0);
		switch (input) {
		case '1':
			showFAT();
			break;
		case '2':
			runCom(getCom());
			FuncClear();
			break;
		case '3':
			getCom();
			runScript();
			FuncClear();
			break;
		case '4':
			FuncClear();
			break;
		case '5':
			runInt();
			FuncClear();
			break;
		case '0':
			FuncPrintSL(mainTips6, strlen(mainTips6));
			return;
		}

	}

}