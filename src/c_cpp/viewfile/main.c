#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROJECT_NAME				"ViewFile"
#ifdef _WIN32
	#define PROJECT_VERSION			"0.1-WinNT"
#else
	#define PROJECT_VERSION			"0.1-GNU/Linux"
#endif
#define PROJECT_YEARS				"2023-2025"
#define AUTHOR						"Mr. ZENG Lai(Zin)"
#define CONTACT						"vip201@126.com"

#include "color.h"
#include "SetCharacterEncode.h"

TextColor failedColor = {COLOR_RED, COLOR_NONE};
TextColor successColor = {COLOR_GREEN, COLOR_NONE};
TextColor warningColor = {COLOR_YELLOW, COLOR_NONE};
TextColor labelColor = {COLOR_BLACK, COLOR_WHITE};

/**
 * @brief 向屏幕打印程序版本等信息
 */
void printVersion(void);

/**
 * @brief 向屏幕打印程序使用指南
 * @param pro 程序所在路径（用于组成包含路径的完整文件名）
 */
void printManual(char* pro);

/**
 * @brief 比较两个字符串是否相同，忽略大小写
 * @param str1 字符串 1
 * @param str2 字符串 2
 * @return 返回 0 表示相同
 */
int StrCmpIgnoreCase(const char *str1, const char *str2);

/**
 * @brief 比较两个字符串的前 n 个字符是否相同，忽略大小写
 * @param str1 字符串 1
 * @param str2 字符串 2
 * @param n 比较字符数
 * @return 返回 0 表示相同
 */
int StrNCmpIgnoreCase(const char *str1, const char *str2, unsigned long long n);

int main(int argc, char* argv[]){ // 开发环境的Win11平台的cmd的type默认为cp=936(gb2312)
	char command[300];

#ifdef _WIN32
	strcpy(command, "type ");
#else
	strcpy(command, "cat ");
#endif

	if(argc<2){
        printVersion();
		ColorPrintf(failedColor, "Too few parameters.\n");
        printManual(argv[0]);
		exit(0);
	}else if(argc==2){ // 未提供以何种代码页查看，则猜测文件是何种编码格式，Automatically Set CodePage
		automaticallySelectCodePage(argv[1]);
		strcat(command, argv[1]);
	}else if(argc==3){
		strcat(command, argv[1]);
		if( !StrCmpIgnoreCase("UTF-8", argv[2]) || !StrCmpIgnoreCase("UTF8", argv[2]) || !StrCmpIgnoreCase("CP65001", argv[2])){
			changeCodePage(65001,65001);
		}else if( !StrCmpIgnoreCase("GB2312", argv[2]) || !StrCmpIgnoreCase("GBK", argv[2]) || !StrCmpIgnoreCase("CP936", argv[2])){
			changeCodePage(936,936);
		}else{
            printVersion();
			ColorPrintf(failedColor, "Invalid parameters: %s\n", argv[2]);
            printManual(argv[0]);
			exit(0);
		}

	}else{
        printVersion();
		ColorPrintf(failedColor, "Too many parameters.\n");
        printManual(argv[0]);
		exit(0);
	}
	int retView = system(command);
	return 0;
}

void printVersion(void) {
	printf("%s version %s (compiled %s, %s)\n", PROJECT_NAME, PROJECT_VERSION, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. All rights reserved. %s\n\n", PROJECT_YEARS, AUTHOR, CONTACT);
}

void printManual(char* pro){
	ColorPrintf(labelColor, " USAGE \n");
    printf("    %s <filename> [OPTIONS]\n\n", pro);

    ColorPrintf(labelColor, " OPTIONS \n");
    printf("    %-26s\t%s\n","[UTF-8 | UTF8 | CP65001]", "Output file contents in UTF-8 encoding format.");
    printf("    %-26s\t%s\n\n","[GB2312 | GBK | CP936]", "Output file contents in GB2312 encoding format.");
}

int StrCmpIgnoreCase(const char *str1, const char *str2) {
	while (*str1 && *str2) {
		char c1 = (*str1 >= 'A' && *str1 <= 'Z') ? (*str1 - 'A' + 'a') : *str1;
		char c2 = (*str2 >= 'A' && *str2 <= 'Z') ? (*str2 - 'A' + 'a') : *str2;
		if (c1 != c2) {
			return c1 - c2; // 返回字符差值，类似于strcmp
		}
		str1++;
		str2++;
	}
	return *str1 - *str2; // 考虑到字符串长度不同的情况
}

int StrNCmpIgnoreCase(const char *str1, const char *str2, unsigned long long n) {
	while (n && *str1 && *str2) {
		char c1 = (*str1 >= 'A' && *str1 <= 'Z') ? (*str1 - 'A' + 'a') : *str1;
		char c2 = (*str2 >= 'A' && *str2 <= 'Z') ? (*str2 - 'A' + 'a') : *str2;
		if (c1 != c2) {
			return c1 - c2; // 返回字符差值
		}
		str1++;
		str2++;
		n--;
	}

	// 如果 n 为 0 或两个字符串在前 n 个字符都相等，则返回 0
	if (n == 0) {
		return 0;
	}

	// 处理字符串长度不同的情况
	return (*str1) ? 1 : ((*str2) ? -1 : 0);
}