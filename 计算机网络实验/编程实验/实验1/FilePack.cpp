#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>

const int nameMaxSize = 300;

struct FileStruct {
	char fileName[nameMaxSize];
	__int64 fileSize;
};

FILE * openFile(const char *pathName, int model) {
	FILE *pFile = NULL;
	if (model == 1) {
		//read
		if ((pFile = fopen(pathName, "rb")) == NULL) {
			printf("\ncan not open the file:%s\n\n", pathName);
			getchar();
			exit(1);
		}
	} else if (model == 0) {
		//write
		if ((pFile = fopen(pathName, "wb")) == NULL) {
			printf("\ncan not open the file:%s\n\n", pathName);
			getchar();
			exit(1);
		}
	}
	return pFile;
}

void closeFile(FILE *pFile) {
	fclose(pFile);
}

__int64 getFileSize(FILE *pFile) {
	fseek(pFile, 0L, SEEK_END);
	return ftell(pFile);
}

char * getFileName(char *pathName) {
	char *pLastSlash = strrchr(pathName, '\\');
	char *pszBaseName = pLastSlash ? pLastSlash + 1 : pathName;
	return pszBaseName;
}
void packFile(struct FileStruct srcInf, FILE * srcFile, FILE * destFile) {
	char *buf;
	fwrite(&srcInf, sizeof(struct FileStruct), 1, destFile);
	buf = (char *)malloc(srcInf.fileSize + 1);
	fseek(srcFile, 0L, SEEK_SET);
	fread(buf, srcInf.fileSize, 1, srcFile);
	fwrite(buf, srcInf.fileSize, 1, destFile);
}

void getUniqueName(char *fileName, int num) {
	//printf("num:%d\n",num);
	char *pBack = strchr(fileName, '.');
	//printf("\nBack:%s\n",pBack);
	char front[nameMaxSize];
	char *pTemp = (num == 2) ? pBack : strrchr(fileName, '(');
	/*printf("\n%d:%s\n", num, pTemp);
	getchar();*/
	strncpy(front, fileName, pTemp - fileName);
	front[pTemp - fileName] = '\0';
	//printf("len:%d\n", pTemp - fileName);
	//printf("\n%d:%s\n", num, front);
	/*printf("%\nlen:%d\n", pBack - fileName);
	printf("%\nlen2:%d\n", strlen(front));
	printf("\nFront:%s\n", front);*/
	char ret[nameMaxSize];
	sprintf(ret, "%s(%d)%s", front, num, pBack);
	//printf("\ntest:%s\n", ret);
	strcpy(fileName, ret);
	/*printf("\ntest:%s\n", fileName);
	getchar();
	getchar();*/

}
int unpackFile(FILE *srcFile, const char *Path) {
	FileStruct fileInfo;
	FILE *destFile;


	while (1) {
		if (fread(&fileInfo, sizeof(FileStruct), 1, srcFile) != 1) {
			printf("\nall files have been unpack\n\n");
			getchar();
			break;
		}
		char *newFileName, fileFullName[nameMaxSize];
		//printf("\n%s  %s\n", Path, fileInfo.fileName);
		//getchar();
		//getchar();
		strcpy(fileFullName, Path);
		strcat(fileFullName, fileInfo.fileName);
		int num = 2;
		while (_access(fileFullName, 0) == 0) {
			//exist
			getUniqueName(fileFullName, num);
			num++;
		}
		destFile = openFile(fileFullName, 0); //open the destFile
		char *buf = (char *)malloc(fileInfo.fileSize + 1);
		if (fread(buf, fileInfo.fileSize, 1, srcFile) != 1) {
			//get the file
			printf("\nreading file unsuccessfully\n\n");
			getchar();
			exit(2);
		}
		if (fwrite(buf, fileInfo.fileSize, 1, destFile) != 1) {
			printf("\nwriting file unsuccessfully\n\n");
			getchar();
			exit(3);
		}
		closeFile(destFile);
	}
}


int main() {
	FILE * pFile1, *pFile2;

	FileStruct file[nameMaxSize];
	char inputBuf[nameMaxSize * 2];
	int num = 0;

	pFile2 = openFile("c:\\test\\FileSet.pak", 0); //write destFile

	while (1) {
		//read srcFile and write it to destFile
		printf("Please input the FileName:");
		scanf_s("%s", inputBuf, nameMaxSize);
		if (strcmp(inputBuf, "exit") == 0) {
			//check if it is teminated
			printf("\nInput has been terminated\n\n");
			break;
		}
		strcpy(file[num].fileName, getFileName(inputBuf));
		//printf("test %d:%s\n", num, file[num].fileName);//test
		//printf("test %d:%s\n", num, inputBuf);//test
		pFile1 = openFile(inputBuf, 1); //read srcFile
		file[num].fileSize = getFileSize(pFile1);//get srcFileSize
		packFile(file[num], pFile1, pFile2);//pack srcFile to srcFile
		closeFile(pFile1);//close srcFile
		fclose(pFile1);
		num++;//inc num
	}
	closeFile(pFile2);//close destFile

	char path[nameMaxSize];
	printf("\nPlease input the unpackFIle path:");
	scanf_s("%s", path, nameMaxSize);
	getchar();
	pFile2 = openFile("c:\\test\\FileSet.pak", 1);//read srcFile
	//const char path[] = "c:\\test\\s\\";
	unpackFile(pFile2, path);

	return 0;
}