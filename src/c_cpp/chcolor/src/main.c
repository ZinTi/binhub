#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// 跨平台的高精度时间函数
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

#include "binhub.h"

#define PROJECT_NAME				"BINHUB ChangeColor"
#ifdef _WIN32
    #define PROJECT_VERSION			"0.0.1-WinNT"
#else
    #define PROJECT_VERSION			"0.0.1-GNU/Linux"
#endif
#define PROJECT_YEARS				"2023-2025"
#define AUTHOR						"Mr. ZENG Lai(Zin)"
#define CONTACT						"Email: vip201@126.com"

binhub_tcolor_t CLR_PROMPT_General    =	{CLR_BRT_WHITE, CLR_NONE};
binhub_tcolor_t CLR_PROMPT_Error      =	{CLR_RED, CLR_NONE};
binhub_tcolor_t CLR_DIGITS            =	{CLR_BRT_GREEN, CLR_NONE};
binhub_tcolor_t CLR_UPPERCASE         =	{CLR_BRT_BLUE, CLR_NONE};
binhub_tcolor_t CLR_LOWERCASE         =	{CLR_BRT_CYAN, CLR_NONE};
binhub_tcolor_t CLR_SPECIAL           =	{CLR_BRT_MAGENTA, CLR_NONE};
binhub_tcolor_t CLR_SerialNumber      = {CLR_BLACK, CLR_BRT_YELLOW};

binhub_tcolor_t CLR_ERROR        = {CLR_BRT_RED, CLR_NONE};

void print_about(void);
void print_manual(char* pro);
int print_main_args(int n,char* v[]);
int print_color_panel(void);
int show_ansi_vt100_color(void);
void put_str(const char* Str);

unsigned long long get_microseconds(); // 获取微秒级时间戳
binhub_tcolor_t binhub_win_attr_to_tcolor(uint8_t color);
binhub_tcolor_t generate_random_front_color(void);

int main(int argc,char* argv[]){
#ifdef _WIN32 // Linux 默认
	binhub_setenc(ENCODING_UTF8);
#endif

    if(argc==2){
        if(!binhub_strcasecmp("red",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_RED, CLR_NONE});
        }else if(!binhub_strcasecmp("green",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_GREEN, CLR_NONE});
        }else if(!binhub_strcasecmp("blue",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BLUE, CLR_NONE});
        }else if(!binhub_strcasecmp("yellow",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_YELLOW, CLR_NONE});
        }else if(!binhub_strcasecmp("purple",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_MAGENTA, CLR_NONE});
        }else if(!binhub_strcasecmp("cyan",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_CYAN, CLR_NONE});
        }else if(!binhub_strcasecmp("white",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_WHITE, CLR_NONE});
        }else if(!binhub_strcasecmp("gray",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_BLACK, CLR_NONE});
        }else if(!binhub_strcasecmp("black",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BLACK, CLR_NONE});
        }else if(!binhub_strcasecmp("lightred",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_RED, CLR_NONE});
        }else if(!binhub_strcasecmp("lightgreen",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_GREEN, CLR_NONE});
        }else if(!binhub_strcasecmp("lightblue",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE});
        }else if(!binhub_strcasecmp("lightyellow",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE});
        }else if(!binhub_strcasecmp("lightpurple",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_MAGENTA, CLR_NONE});
        }else if(!binhub_strcasecmp("lightcyan",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE});
        }else if(!binhub_strcasecmp("lightwhite",argv[1])){
            binhub_chcolor((binhub_tcolor_t){CLR_BRT_WHITE, CLR_NONE});
        }else if(!binhub_strcasecmp("random",argv[1])){
            binhub_chcolor(generate_random_front_color());
        }else if(!strcmp("--help",argv[1])||!strcmp("-?",argv[1])){
            print_manual(argv[0]);
        }else if(!strcmp("--list",argv[1])||!strcmp("-l",argv[1])){
            print_color_panel();
            show_ansi_vt100_color();
        }else if(!strcmp("--version",argv[1])||!strcmp("-v",argv[1])){
            print_about();
        }else if(strlen(argv[1])==2){
            if( ((argv[1][0]>='0' && argv[1][0]<='9') || (argv[1][0]>='A' && argv[1][0]<='F') || (argv[1][0]>='a' && argv[1][0]<='f')) &&
                ((argv[1][1]>='0' && argv[1][1]<='9') || (argv[1][1]>='A' && argv[1][1]<='F') || (argv[1][1]>='a' && argv[1][1]<='f')) ){
                    long win32_color_attr;
                    binhub_strtol(argv[1], &win32_color_attr);
                    binhub_tcolor_t color_value = binhub_win_attr_to_tcolor((uint8_t)(win32_color_attr));
                    binhub_chcolor(color_value);
            }else {
                binhub_cprintf(CLR_ERROR, "无效参数！使用--help或-?参数获取帮助。"); BINHUB_ENDLINE(1);
            }
        }else {
            binhub_cprintf(CLR_ERROR, "无效参数！使用--help或-?参数获取帮助。"); BINHUB_ENDLINE(1);
        }
    }else if(argc<2){
        print_about();
        binhub_cprintf(CLR_ERROR, "参数过少！使用--help或-?参数获取帮助。"); BINHUB_ENDLINE(1);
    }else {
        print_main_args(argc, argv);
        binhub_cprintf(CLR_ERROR, "参数过多！使用--help或-?参数获取帮助。"); BINHUB_ENDLINE(1);
    }
    return 0;
}

void print_about(void){
    printf("%s version %s (compiled %s, %s)\n", PROJECT_NAME, PROJECT_VERSION, __TIME__, __DATE__);
    printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n", PROJECT_YEARS, AUTHOR);
    printf("%s\n\n", CONTACT);
}

void print_manual(char* pro){
    puts("[Usage]");
    printf("%4s%s %s\n\n"," ", pro, "[OPTIONS]");
    puts("[Options]");
    printf("%4s%-13s%s\n", " ", "--help","显示用户帮助");
    printf("%4s%-13s%s\n"," ","--list","列出 Win32 API 颜色和 ANSI/VT100 控制码");
    printf("%4s%-13s%s\n\n", " ", "--version","显示版本信息");

    printf("%4s%-13s%s\n", " ", "[attr]","指定shell输出的颜色属性");
    printf("%17s%s\n"," ","(1) 由两个十六进制数字组成，例如：");
    printf("%21s%-10s%s"," ","DF","指定shell输出颜色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_WHITE, CLR_BRT_MAGENTA}, "亮紫色背景亮白色前景"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","E9","指定shell输出颜色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_BRT_YELLOW},"亮黄色背景亮蓝色前景"); BINHUB_ENDLINE(2);

    printf("%17s%s\n"," ","(2) 可选择以下属性：");
    printf("%21s%-10s%s"," ","red", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_RED, CLR_NONE},"红色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","green", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_GREEN, CLR_NONE},"绿色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","blue", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BLUE, CLR_NONE},"蓝色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","yellow", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_YELLOW, CLR_NONE},"黄色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","purple", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_MAGENTA, CLR_NONE},"紫色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","cyan", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_CYAN, CLR_NONE},"青色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","white", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_WHITE, CLR_NONE},"白色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","gray", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLACK, CLR_NONE},"灰色"); BINHUB_ENDLINE(1);
    printf("%21s%-10s%s"," ","black", "指定shell输出颜色为"); binhub_cprintf((binhub_tcolor_t){CLR_BLACK, CLR_WHITE},"白底黑字"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightred", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_RED, CLR_NONE},"亮红色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightgreen", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_GREEN, CLR_NONE},"亮绿色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightblue", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE},"亮蓝色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightyellow", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE},"亮黄色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightpurple", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_MAGENTA, CLR_NONE},"亮紫色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightcyan", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE},"亮青色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","lightwhite", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t){CLR_BRT_WHITE, CLR_NONE},"亮白色"); BINHUB_ENDLINE(1);
    printf("%21s%-15s%s"," ","random", "指定shell输出前景色为"); binhub_cprintf((binhub_tcolor_t)(generate_random_front_color()),"随机色"); BINHUB_ENDLINE(2);

    printf("%4s%-13s%s\n", " ", "-?","显示用户帮助");
    printf("%4s%-13s%s\n"," ","-l","列出 Win32 API 颜色和 ANSI/VT100 控制码");
    printf("%4s%-13s%s\n\n", " ", "-v","显示版本信息");

}

int print_color_panel(void){
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE}, "------ Win32 API 颜色属性十六进制表示 ------"); BINHUB_ENDLINE(1);
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_BRT_YELLOW}, "       前景色       \t       背景色       "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_BLACK, CLR_NONE}," 黑色0x00 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLACK, CLR_NONE}, " 灰色0x08 \t\t  ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_BLACK}, " 灰色0x80 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_BLUE, CLR_NONE}, " 蓝色0x01 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE}, " 亮蓝0x09 \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BLUE}, " 蓝色0x10 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_BLUE}, " 亮蓝0x90 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_GREEN, CLR_NONE}, " 绿色0x02 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_GREEN, CLR_NONE}, " 亮绿0x0A \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_GREEN}, " 绿色0x20 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_GREEN}, " 亮绿0xA0 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_CYAN, CLR_NONE}, " 青色0x03 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_CYAN, CLR_NONE}, " 亮青0x0B \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_CYAN}, " 青色0x30 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_CYAN}, " 亮青0xB0 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_RED, CLR_NONE}, " 红色0x04 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_RED, CLR_NONE}, " 亮红0x0C \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_RED}, " 红色0x40 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_RED}, " 亮红0xC0 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_MAGENTA, CLR_NONE}, " 紫色0x05 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_MAGENTA, CLR_NONE}, " 亮紫0x0D \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_MAGENTA}, " 紫色0x50 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_MAGENTA}, " 亮紫0xD0 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_YELLOW, CLR_NONE}, " 黄色0x06 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_YELLOW, CLR_NONE}, " 亮黄0x0E \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_YELLOW}, " 黄色0x60 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_YELLOW}, " 亮黄0xE0 "); BINHUB_ENDLINE(1);

    binhub_cprintf((binhub_tcolor_t){CLR_WHITE, CLR_NONE}, " 白色0x07 ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_WHITE, CLR_NONE}, " 亮白0x0F \t");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_WHITE}, " 白色0x70 ");
    binhub_cprintf((binhub_tcolor_t){CLR_RESET, CLR_BRT_WHITE}, " 亮白0xF0 "); BINHUB_ENDLINE(2);

    return 0;
}

int show_ansi_vt100_color(void){
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE}, "------------ ANSI/VT100 控制码 ------------"); BINHUB_ENDLINE(1);
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLACK, CLR_NONE}, "* 若以下字体颜色未能正确显示，说明当前窗口不支持解析“控制字符序列”输出！"); BINHUB_ENDLINE(1);
    printf("\e[94;103m     前景色     \e[0m \e[94;103m     背景色      \e[0m \e[94;103m  描述  \e[0m\n");
    printf("\e[30m \\e[30m \e[0m\e[90m \\e[90m \e[0m \e[40m \\e[40m \e[0m\e[100m \\e[100m \e[0m  黑色系\n");
    printf("\e[31m \\e[31m \e[0m\e[91m \\e[91m \e[0m \e[41m \\e[41m \e[0m\e[101m \\e[101m \e[0m  红色系\n");
    printf("\e[32m \\e[32m \e[0m\e[92m \\e[92m \e[0m \e[42m \\e[42m \e[0m\e[102m \\e[102m \e[0m  绿色系\n");
    printf("\e[33m \\e[33m \e[0m\e[93m \\e[93m \e[0m \e[43m \\e[43m \e[0m\e[103m \\e[103m \e[0m  黄色系\n");
    printf("\e[34m \\e[34m \e[0m\e[94m \\e[94m \e[0m \e[44m \\e[44m \e[0m\e[104m \\e[104m \e[0m  蓝色系\n");
    printf("\e[35m \\e[35m \e[0m\e[95m \\e[95m \e[0m \e[45m \\e[45m \e[0m\e[105m \\e[105m \e[0m  紫色系\n");
    printf("\e[36m \\e[36m \e[0m\e[96m \\e[96m \e[0m \e[46m \\e[46m \e[0m\e[106m \\e[106m \e[0m  青色系\n");
    printf("\e[37m \\e[37m \e[0m\e[97m \\e[97m \e[0m \e[47m \\e[47m \e[0m\e[107m \\e[107m \e[0m  白色系\n\n");
    return 0;
}

int print_main_args(const int n,char* v[]){
    puts("--------- Arguments ---------");
    for(int i=0;i<n;i++){
        printf("%02d | %p | %s\n",i,v[i],v[i]);
    }
    puts("-----------------------------");
    return 0;
}

void put_str(const char* Str){ // puts()末尾自动换行，printf()不想使用
    for(int i=0;;++i){
        if(*(Str+i)!='\0')
            putchar(*(Str+i));
        else
            break;
    }
}

// 获取微秒级时间戳
unsigned long long get_microseconds() {
#if defined(_WIN32) || defined(_WIN64)
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    unsigned long long tt = ft.dwHighDateTime;
    tt <<= 32;
    tt |= ft.dwLowDateTime;
    tt /= 10;  // 转换为微秒
    tt -= 11644473600000000ULL;  // 调整为Unix纪元
    return tt;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long long)tv.tv_sec * 1000000ULL + tv.tv_usec;
#endif
}

// 生成随机前景色
binhub_tcolor_t generate_random_front_color(void){
    // 使用微秒级时间初始化随机种子
    unsigned long long seed = get_microseconds();
    srand((unsigned int)seed);
    uint8_t random_value = rand() % 16; // 生成0-15的随机整数

    // 设置颜色
    binhub_tcolor_t color;
    color.fg = (binhub_ansi_color_t)random_value;
    if (random_value == 0) {
        color.bg = CLR_BRT_WHITE;
    } else { // random_value ∈ [1, 15]
        color.bg = CLR_NONE;
    }
    return color;
}
