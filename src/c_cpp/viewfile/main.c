#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binhub.h"

#define PROJECT_NAME				"BINHUB ViewFile"
#ifdef _WIN32
	#define PROJECT_VERSION			"0.0.1-WinNT"
#else
	#define PROJECT_VERSION			"0.0.1-GNU/Linux"
#endif
#define PROJECT_YEARS				"2023-2025"
#define AUTHOR						"Mr. ZENG Lai(Zin)"
#define CONTACT						"vip201@126.com"

#include "binhub.h"

binhub_tcolor_t color_failed = {CLR_RED, CLR_NONE};
binhub_tcolor_t color_success = {CLR_GREEN, CLR_NONE};
binhub_tcolor_t color_warning = {CLR_YELLOW, CLR_NONE};
binhub_tcolor_t color_label = {CLR_BLACK, CLR_WHITE};

/**
 * @brief 向屏幕打印程序版本等信息
 */
void print_version(void);

/**
 * @brief 向屏幕打印程序使用指南
 * @param pro 程序所在路径（用于组成包含路径的完整文件名）
 */
void print_manual(char* pro);

int main(int argc, char* argv[]){ // 开发环境的Win11平台的cmd的type默认为cp=936(gb2312)
	char command[300];

#ifdef _WIN32
	strcpy(command, "type ");
#else
	strcpy(command, "cat ");
#endif

	if(argc<2){
        print_version();
		binhub_cprintf(color_failed, "Too few parameters.\n");
        print_manual(argv[0]);
		exit(0);
	}else if(argc==2){ // 未提供以何种代码页查看，则猜测文件是何种编码格式，Automatically Set CodePage
		binhub_setcp_auto(argv[1]);
		strcat(command, argv[1]);
	}else if(argc==3){
		strcat(command, argv[1]);
		if( !binhub_strcasecmp("UTF-8", argv[2]) || !binhub_strcasecmp("UTF8", argv[2]) || !binhub_strcasecmp("CP65001", argv[2])){
			binhub_setenc(ENCODING_UTF8);
		}else if( !binhub_strcasecmp("GB2312", argv[2]) || !binhub_strcasecmp("GBK", argv[2]) || !binhub_strcasecmp("CP936", argv[2])){
			binhub_setenc(ENCODING_GBK);
		}else {
            print_version();
			binhub_cprintf(color_failed, "Invalid parameters: %s\n", argv[2]);
            print_manual(argv[0]);
			exit(0);
		}

	}else {
        print_version();
		binhub_cprintf(color_failed, "Too many parameters.\n");
        print_manual(argv[0]);
		exit(0);
	}
	int ret_view = system(command);
	return 0;
}

void print_version(void) {
	printf("%s version %s (compiled %s, %s)\n", PROJECT_NAME, PROJECT_VERSION, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. All rights reserved. %s\n\n", PROJECT_YEARS, AUTHOR, CONTACT);
}

void print_manual(char* pro){
	binhub_cprintf(color_label, " USAGE \n");
    printf("    %s <filename> [OPTIONS]\n\n", pro);

    binhub_cprintf(color_label, " OPTIONS \n");
    printf("    %-26s\t%s\n","[UTF-8 | UTF8 | CP65001]", "Output file contents in UTF-8 encoding format.");
    printf("    %-26s\t%s\n\n","[GB2312 | GBK | CP936]", "Output file contents in GB2312 encoding format.");
}
