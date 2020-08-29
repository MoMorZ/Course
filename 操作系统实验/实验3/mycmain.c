__asm__(".code16gcc\r\n");
typedef unsigned int size_t;
#define NULL 0

extern void FuncPrintC();
extern void FuncPrintS();
extern void FuncPrintSL();
extern void FuncClear();
extern void FuncGetChar();
extern void FuncReadFAT();
extern void FuncRunP();
extern void FuncReadFile();

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

//局部可能用到的
int i = 0, j = 0, len = 0;

//函数声明
void *memset(void *s, int c, size_t n) ;
void* memmove(void*, const void*, size_t);
char* fstrcpy(char* dest, const char* src);
char* fstrncpy(char* dest, const char* src, int len);
int strcmp(const char * d, const char * s);
size_t strlen(const char * s);
void runProg();
void getCom();
void runCom();
void getFile();
void readFile();
void mycmain();
void showFAT();

//前8字节文件名，9、10文件所在扇区，11、12文件大小，13、14文件创建时间，15，16文件类型

const char MessHello[40] = "Hello,OS!";

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

void runCom() {
	FuncPrintSL(RunTips1, strlen(RunTips1));
	memset(FileName, 0, 8);
	i = 0, j = 0;
	len = strlen(Command);
	//FuncPrintSL("len:", 4);

	while (i < len) {
		j = i;
		//FuncPrintC(i + 48);
		//FuncPrintSL("", 0);
		while (Command[j] != ' ' && Command[j] != '\0') {
			j++;
		}
		//FuncPrintC(j + 48);
		//FuncPrintSL("", 0);
		fstrncpy(FileName, Command + i, j - i);
		FuncPrintSL(FileName, strlen(FileName));
		getFile();
		runProg();
		i = j + 1;
	}
	FuncPrintSL(RunTips3, strlen(RunTips3));
	return;
}

const char FileTips1[] = "Tring to get file...";

void getFile() {
	FuncPrintSL(FileTips1, strlen(FileTips1));
	FuncReadFAT(FAT);
	InfoCount = 0;
	do {
		fstrncpy(temp, FAT + InfoCount, 8);
		SecNo = FAT[InfoCount + 9];//取得扇区号
		//FuncPrintSL("temp:", 5);
		//FuncPrintSL(temp, 8);
		diff = strcmp(temp, FileName);
		InfoCount += 16;
	} while ( (InfoCount < (NumFile * 16)) && (diff != 0));
	//FuncPrintSL("SecNo:", 6);
	//FuncPrintC(SecNo + 55);
	//FuncPrintSL("", 0);
	return;
}

const char ReadTips1[] = "Read file successfully!";

void readFile() {
	FuncReadFile(buffer, SecNo);
	FuncPrintSL(ReadTips1, strlen(ReadTips1));
	return;
}



const char ProgTips1[] = "Tring to load progress...";
const char ProgTips2[] = "Load progress successfully!";

void runProg() {
	//BuffLen = strlen(buffer);
	FuncPrintSL(ProgTips1, strlen(ProgTips1));
	FuncRunP(SecNo);
	FuncPrintSL(ProgTips2, strlen(ProgTips2));
	return;
}


const char Comtips1[] = "Please input your command:";

void getCom() {
	/*读取批处理命令部分*/
	BuffLen = strlen(Comtips1);
	FuncPrintSL(Comtips1, BuffLen);
	memset(Command, 0, 50);
	i = 0;
	while (1) {
		FuncGetChar();
		FuncPrintC(input);
		if (input == '\n' || input == '\r') break;
		//FuncPrintC(input);
		Command[i++] = input;
	}
	Command[i + 1] = '\0';
	//FuncPrintSL(Command, strlen(Command));
	return;
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
	getFile();
	readFile();
	fstrncpy(Command, buffer, 50);
//	FuncPrintSL("test2:", 6);
//	FuncPrintSL(buffer, 512);
//	FuncPrintSL(Command, strlen(Command));
	runCom();
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

	while (i < 16 * NumFile) {
		FuncPrintS(FAT + i, 8);
		input = FAT[i + 9];
		if (input < 10) {
			input += 48;
		} else if (input < 16) {
			input += 55;
		}
		FuncPrintC(input);
		i += 16;
	}
	FuncPrintSL("", 0);
	return;
}

const char mainTips1[] = "Please input number to choose the function";
const char mainTips2[] = "1.Show the FAT";
const char mainTips3[] = "2.Run Command";
const char mainTips4[] = "3.Run Script";
const char mainTips5[] = "0.Exit";
const char mainTips6[] = "END!";
const char mainTips7[] = "4.Clear Screen";

void mycmain() {
	//欢迎部分
	FuncPrintSL(MessHello, strlen(MessHello));
	FuncPrintSL(mainTips1, strlen(mainTips1));
	FuncPrintSL(mainTips2, strlen(mainTips2));
	FuncPrintSL(mainTips3, strlen(mainTips3));
	FuncPrintSL(mainTips4, strlen(mainTips4));
	FuncPrintSL(mainTips7, strlen(mainTips7));
	FuncPrintSL(mainTips5, strlen(mainTips5));

	while (1) {
		FuncGetChar();
		FuncPrintC(input);
		FuncPrintSL("", 0);
		switch (input) {
		case '1':
			showFAT();
			break;
		case '2':
			getCom();
			runCom();
			break;
		case '3':
			getCom();
			runScript();
			break;
		case '4':
			FuncClear();
			break;
		case '0':
			FuncPrintSL(mainTips6, strlen(mainTips6));
			return;
		}
	}

}