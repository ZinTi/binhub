#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "SetCharacterEncode.h"
#include "color.h"

#define TEMPLATE_CR_YEARS   "2025"


int main(int argc, char* argv[]){
    changeCodePage(65001, 65001);
	if(argc!=2){
		printManual(argv[0]);
	}else{
		addCopyleft(argv[1]);
	}
    changeCodePage(0, 0);
	return 0;
}

void printManual(const char* mPath){
    printf("%s version %s (compiled %s, %s)\n", PROJ_NAME, PROJ_VER, __TIME__, __DATE__);
    printf("Copyleft (c) %s %s. All rights reserved. %s\n\n", PROJ_YEARS, AUTHOR, AUTHOR_CONTACT);

    printf("Usage: %s <filename>\n\n", mPath);

    const TextColor failedColor = { COLOR_BRIGHT_RED, COLOR_NONE };
    ColorPrintf(failedColor, "Please enter the parameters!\n");
}

int addCopyleft(const char* srcFilename){
    int returnValue = 0; // 忽略返回值
    // 备份文件
    char* cmd = (char*)malloc( strlen(srcFilename)*2 + 11 );
    if(cmd!=NULL){
#ifdef _WIN32
        sprintf(cmd, "copy %s %s.bak", srcFilename, srcFilename);
#else
        sprintf(cmd, "cp %s %s.bak", srcFilename, srcFilename);
#endif
        returnValue = system(cmd);
        free(cmd);
    }else{
        perror("Memory allocation error");
        exit(-1);
    }

	char description[1000];
    automaticallySelectCodePage(srcFilename); // 根据目标源文件改变CP
    printf("Description > ");
    returnValue = scanf("%s", description);

	char version[100];
	printf("Version > ");
	returnValue = scanf("%s", version);
    changeCodePage(0, 0); // 恢复CP

	time_t now_time;
	time(&now_time);

    char new_data[4096];
	sprintf(new_data, "/***********************************************************\n \
 * Copyleft (c) "TEMPLATE_CR_YEARS" "AUTHOR". All rights reserved.\n \
 * \n \
 * File: %s\n \
 * Description: %s\n \
 * Version: %s\n \
 * Author: "AUTHOR" (Zin)\n \
 * Contact: "AUTHOR_CONTACT"\n \
 * Last Modified: %s \
 * \n \
***********************************************************/\n", srcFilename, description, version, ctime(&now_time));

    FILE *fp = fopen(srcFilename, "r+b");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // 获取文件原始数据大小
    fseek(fp, 0, SEEK_END);
    const long original_size = ftell(fp);
    rewind(fp);

    char* original_data = (char *)malloc(original_size + 1); // 加上1是为了存储字符串结束符'\0'
    if (original_data != NULL) {
        returnValue = fread(original_data, 1, original_size, fp); // 读取文件原始数据到内存中
    }else{
        perror("Memory allocation error");
        exit(-1);
    }
    fclose(fp);

    fp = fopen(srcFilename, "w+b"); // 打开文件以写入新数据
    if (fp != NULL) {
        fwrite(new_data, 1, strlen(new_data), fp); // 写入新数据
        fwrite(original_data, 1, original_size, fp); // 写入原始数据
    }else{
        perror("Error opening file");
        exit(-1);
    }

    fclose(fp);
    free(original_data);

    const TextColor successColor = { COLOR_BRIGHT_BLACK, COLOR_BRIGHT_GREEN };
    ColorPrintf(successColor, "Data appended successfully.\n");

    return 0;
}

