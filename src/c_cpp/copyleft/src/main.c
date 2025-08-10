#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "binhub.h"

#define TEMPLATE_CR_YEARS   "2025"


int main(int argc, char* argv[]){
#ifdef _WIN32 // Linux 默认
    binhub_setenc(ENCODING_UTF8);
#endif
	if(argc!=2){
		print_manual(argv[0]);
	}else {
		add_copyleft(argv[1]);
	}

	return 0;
}

void print_manual(const char* module_path){
    printf("%s version %s (compiled %s, %s)\n", PROJ_NAME, PROJ_VER, __TIME__, __DATE__);
    printf("Copyleft (c) %s %s. All rights reserved. %s\n\n", PROJ_YEARS, AUTHOR, AUTHOR_CONTACT);

    printf("Usage: %s <filename>\n\n", module_path);

    binhub_cprintf((binhub_tcolor_t){ CLR_BRT_RED, CLR_NONE }, "Please enter the parameters!"); BINHUB_ENDLINE(1);
}

int add_copyleft(const char* src_fn){
    // 备份文件
    char* cmd = (char*)malloc( strlen(src_fn)*2 + 11 );
    if(cmd!=NULL){
#ifdef _WIN32
        sprintf(cmd, "copy %s %s.bak", src_fn, src_fn);
#else
        sprintf(cmd, "cp %s %s.bak", src_fn, src_fn);
#endif
        BINHUB_UNUSED(system(cmd));
        free(cmd);
    }else {
        perror("Memory allocation error");
        exit(-1);
    }

	char description[1000];
    binhub_setcp_auto(src_fn); // 根据目标源文件改变CP
    printf("Description > ");
    BINHUB_UNUSED(scanf("%s", description));

	char version[100];
	printf("Version > ");
	BINHUB_UNUSED(scanf("%s", version));

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
***********************************************************/\n", src_fn, description, version, ctime(&now_time));

    FILE *fp = fopen(src_fn, "r+b");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // 获取文件原始数据大小
    fseek(fp, 0, SEEK_END);
    const long original_size = ftell(fp);
    rewind(fp);

    char* original_data = (char* )malloc(original_size + 1); // 加上1是为了存储字符串结束符'\0'
    if (original_data != NULL) {
        BINHUB_UNUSED(fread(original_data, 1, original_size, fp)); // 读取文件原始数据到内存中
    }else {
        perror("Memory allocation error");
        exit(-1);
    }
    fclose(fp);

    fp = fopen(src_fn, "w+b"); // 打开文件以写入新数据
    if (fp != NULL) {
        fwrite(new_data, 1, strlen(new_data), fp); // 写入新数据
        fwrite(original_data, 1, original_size, fp); // 写入原始数据
    }else {
        perror("Error opening file");
        exit(-1);
    }

    fclose(fp);
    free(original_data);

    const binhub_tcolor_t color_success = { CLR_BRT_BLACK, CLR_BRT_GREEN };
    binhub_cprintf(color_success, "Data appended successfully.");
    BINHUB_ENDLINE(1);

    return 0;
}

