// 系统库
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#endif

#define PROJECT_NAME				"ChangeColor"
#ifdef _WIN32
    #define PROJECT_VERSION			"0.1-WinNT"
#else
    #define PROJECT_VERSION			"0.1-GNU/Linux"
#endif
#define PROJECT_YEARS				"2023-2025"
#define AUTHOR						"Mr. ZENG Lai(Zin)"
#define CONTACT						"Email: vip201@126.com"

#define F_Red            0x04
#define F_Green          0x02
#define F_Blue           0x01
#define F_Yellow         0x06
#define F_Purple         0x05
#define F_Cyan           0x03
#define F_White          0x07
#define F_Gray           0x08
#define F_Black          0x70
#define F_LightRed       0x0C
#define F_LightGreen     0x0A
#define F_LightBlue      0x09
#define F_LightYellow    0x0E
#define F_LightPurple    0x0D
#define F_LightCyan      0x0B
#define F_LightWhite     0x0F

void About(void);
void Manual(char* pro);
int ShowMainArgs(int n,char* v[]);
int ShowColorPanel(void);
int ShowANSI_VT100_Color(void);
int ConvertHex(const char* hexadecimalStr, long* pDecimal);
int ColorPrintf(unsigned short color, const char* format, ...);
int ColorPuts(unsigned short color, const char* Str, unsigned short line);
int ChColor(unsigned short color, unsigned short* pOldColor);
int ChCP(unsigned int newInputCP, unsigned int newOutputCP);
int StrCmpIgnoreCase(const char *str1, const char *str2);
void PutStr(const char* Str);

int main(int argc,char* argv[]){
    ChCP(65001, 65001);

    unsigned short OldColor;
    if(argc==2){
        if(!StrCmpIgnoreCase("red",argv[1])){
            ChColor(F_Red, &OldColor);
        }else if(!StrCmpIgnoreCase("green",argv[1])){
            ChColor(F_Green, &OldColor);
        }else if(!StrCmpIgnoreCase("blue",argv[1])){
            ChColor(F_Blue, &OldColor);
        }else if(!StrCmpIgnoreCase("yellow",argv[1])){
            ChColor(F_Yellow, &OldColor);
        }else if(!StrCmpIgnoreCase("purple",argv[1])){
            ChColor(F_Purple, &OldColor);
        }else if(!StrCmpIgnoreCase("cyan",argv[1])){
            ChColor(F_Cyan, &OldColor);
        }else if(!StrCmpIgnoreCase("white",argv[1])){
            ChColor(F_White, &OldColor);
        }else if(!StrCmpIgnoreCase("gray",argv[1])){
            ChColor(F_Gray, &OldColor);
        }else if(!StrCmpIgnoreCase("black",argv[1])){
            ChColor(F_Black, &OldColor);
        }else if(!StrCmpIgnoreCase("lightred",argv[1])){
            ChColor(F_LightRed, &OldColor);
        }else if(!StrCmpIgnoreCase("lightgreen",argv[1])){
            ChColor(F_LightGreen, &OldColor);
        }else if(!StrCmpIgnoreCase("lightblue",argv[1])){
            ChColor(F_LightBlue, &OldColor);
        }else if(!StrCmpIgnoreCase("lightyellow",argv[1])){
            ChColor(F_LightYellow, &OldColor);
        }else if(!StrCmpIgnoreCase("lightpurple",argv[1])){
            ChColor(F_LightPurple, &OldColor);
        }else if(!StrCmpIgnoreCase("lightcyan",argv[1])){
            ChColor(F_LightCyan, &OldColor);
        }else if(!StrCmpIgnoreCase("lightwhite",argv[1])){
            ChColor(F_LightWhite, &OldColor);
        }else if(!strcmp("--help",argv[1])||!strcmp("-?",argv[1])){
            Manual(argv[0]);
        }else if(!strcmp("--list",argv[1])||!strcmp("-l",argv[1])){
            ShowColorPanel();
            ShowANSI_VT100_Color();
        }else if(!strcmp("--version",argv[1])||!strcmp("-v",argv[1])){
            About();
        }else if(strlen(argv[1])==2){
            if( argv[1][0]>='0' && argv[1][0]<='9' || argv[1][0]>='A' && argv[1][0]<='F' || argv[1][0]>='a' && argv[1][0]<='f' &&
                argv[1][1]>='0' && argv[1][1]<='9' || argv[1][1]>='A' && argv[1][1]<='F' || argv[1][1]>='a' && argv[1][1]<='f' ){
                    long value;
                    ConvertHex(argv[1], &value);
                    ChColor(value, &OldColor);
            }else{
                ColorPuts(0x0C,"无效参数！使用--help或-?参数获取帮助。", 1);
            }
        }else{
            ColorPuts(0x0C,"无效参数！使用--help或-?参数获取帮助。", 1);
        }
    }else if(argc<2){
        About();
        ColorPuts(0x0C,"参数过少！使用--help或-?参数获取帮助。", 1);
    }else{
        ShowMainArgs(argc, argv);
        ColorPuts(0x0C,"参数过多！使用--help或-?参数获取帮助。", 1);
    }
    ChCP(0, 0); // 恢复
    return 0;
}

void About(void){
    printf("%s version %s (compiled %s, %s)\n", PROJECT_NAME, PROJECT_VERSION, __TIME__, __DATE__);
    printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n", PROJECT_YEARS, AUTHOR);
    printf("%s\n\n", CONTACT);
}

void Manual(char* pro){
    puts("[Usage]");
    printf("%4s%s %s\n\n"," ", pro, "[OPTIONS]");
    puts("[Options]");
    printf("%4s%-13s%s\n", " ", "--help","显示用户帮助");
    printf("%4s%-13s%s\n"," ","--list","列出 Win32 API 颜色和 ANSI/VT100 控制码");
    printf("%4s%-13s%s\n\n", " ", "--version","显示版本信息");

    printf("%4s%-13s%s\n", " ", "[attr]","指定shell输出的颜色属性");
    printf("%17s%s\n"," ","(1) 由两个十六进制数字组成，例如：");
    printf("%21s%-10s%s"," ","DF","指定shell输出颜色为"); ColorPuts(0xDF,"浅紫色背景亮白色前景", 1);
    printf("%21s%-10s%s"," ","E9","指定shell输出颜色为"); ColorPuts(0xE9,"浅黄色背景浅蓝色前景",2);

    printf("%17s%s\n"," ","(2) 可选择以下属性：");
    printf("%21s%-10s%s"," ","red", "指定shell输出前景色为"); ColorPuts(F_Red,"红色", 1);
    printf("%21s%-10s%s"," ","green", "指定shell输出前景色为"); ColorPuts(F_Green,"绿色", 1);
    printf("%21s%-10s%s"," ","blue", "指定shell输出前景色为"); ColorPuts(F_Blue,"蓝色", 1);
    printf("%21s%-10s%s"," ","yellow", "指定shell输出前景色为"); ColorPuts(F_Yellow,"黄色", 1);
    printf("%21s%-10s%s"," ","purple", "指定shell输出前景色为"); ColorPuts(F_Purple,"紫色", 1);
    printf("%21s%-10s%s"," ","cyan", "指定shell输出前景色为"); ColorPuts(F_Cyan,"青色", 1);
    printf("%21s%-10s%s"," ","white", "指定shell输出前景色为"); ColorPuts(F_White,"白色", 1);
    printf("%21s%-10s%s"," ","gray", "指定shell输出前景色为"); ColorPuts(F_Gray,"灰色", 1);
    printf("%21s%-10s%s"," ","black", "指定shell输出颜色为"); ColorPuts(F_Black,"白底黑字", 1);
    printf("%21s%-15s%s"," ","lightred", "指定shell输出前景色为"); ColorPuts(F_LightRed,"浅红色", 1);
    printf("%21s%-15s%s"," ","lightgreen", "指定shell输出前景色为"); ColorPuts(F_LightGreen,"浅绿色", 1);
    printf("%21s%-15s%s"," ","lightblue", "指定shell输出前景色为"); ColorPuts(F_LightBlue,"浅蓝色", 1);
    printf("%21s%-15s%s"," ","lightyellow", "指定shell输出前景色为"); ColorPuts(F_LightYellow,"浅黄色", 1);
    printf("%21s%-15s%s"," ","lightpurple", "指定shell输出前景色为"); ColorPuts(F_LightPurple,"浅紫色", 1);
    printf("%21s%-15s%s"," ","lightcyan", "指定shell输出前景色为"); ColorPuts(F_LightCyan,"浅青色", 1);
    printf("%21s%-15s%s"," ","lightwhite", "指定shell输出前景色为"); ColorPuts(F_LightWhite,"亮白色",2);

    printf("%4s%-13s%s\n", " ", "-?","显示用户帮助");
    printf("%4s%-13s%s\n"," ","-l","列出 Win32 API 颜色和 ANSI/VT100 控制码");
    printf("%4s%-13s%s\n\n", " ", "-v","显示版本信息");

}

int ShowColorPanel(void){
    ColorPuts(0x09, "------ Win32 API 颜色属性十六进制表示 ------", 1);
    ColorPuts(0xE9, "       前景色       \t       背景色       ", 1);

    ColorPuts(0x70," 黑色0x00 ", 0);
    ColorPuts(0x08, " 灰色0x08 \t\t  ", 0);
    ColorPuts(0x80, " 灰色0x80 ", 1);
    
    ColorPuts(0x01, " 蓝色0x01 ", 0);
    ColorPuts(0x09, " 浅蓝0x09 \t", 0);
    ColorPuts(0x10, " 蓝色0x10 ", 0);
    ColorPuts(0x90, " 浅蓝0x90 ", 1);
    
    ColorPuts(0x02, " 绿色0x02 ", 0);
    ColorPuts(0x0A, " 浅绿0x0A \t", 0);
    ColorPuts(0x20, " 绿色0x20 ", 0);
    ColorPuts(0xA0, " 浅绿0xA0 ", 1);

    ColorPuts(0x03, " 青色0x03 ", 0);
    ColorPuts(0x0B, " 浅青0x0B \t", 0);
    ColorPuts(0x30, " 青色0x30 ", 0);
    ColorPuts(0xB0, " 浅青0xB0 ", 1);

    ColorPuts(0x04, " 红色0x04 ", 0);
    ColorPuts(0x0C, " 浅红0x0C \t", 0);
    ColorPuts(0x40, " 红色0x40 ", 0);
    ColorPuts(0xC0, " 浅红0xC0 ", 1);

    ColorPuts(0x05, " 紫色0x05 ", 0);
    ColorPuts(0x0D, " 浅紫0x0D \t", 0);
    ColorPuts(0x50, " 紫色0x50 ", 0);
    ColorPuts(0xD0, " 浅紫0xD0 ", 1);

    ColorPuts(0x06, " 黄色0x06 ", 0);
    ColorPuts(0x0E, " 浅黄0x0E \t", 0);
    ColorPuts(0x60, " 黄色0x60 ", 0);
    ColorPuts(0xE0, " 浅黄0xE0 ", 1);

    ColorPuts(0x07, " 白色0x07 ", 0);
    ColorPuts(0x0F, " 亮白0x0F \t", 0);
    ColorPuts(0x70, " 白色0x70 ", 0);
    ColorPuts(0xF0, " 亮白0xF0 ", 2);

    return 0;
}

int ShowANSI_VT100_Color(void){
    ColorPuts(0x09, "------------ ANSI/VT100 控制码 ------------", 1);
    ColorPuts(0x08, "* 若以下字体颜色未能正确显示，说明当前窗口不支持解析“控制字符序列”输出！", 1);
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

int ShowMainArgs(const int n,char* v[]){
    puts("--------- Arguments ---------");
    for(int i=0;i<n;i++){
        printf("%02d | %p | %s\n",i,v[i],v[i]);
    }
    puts("-----------------------------");
    return 0;
}

int ConvertHex(const char* hexadecimalStr, long* pDecimal){
    char* endptr;
    strtol(hexadecimalStr, &endptr, 16);
    if (*endptr != '\0') {
        puts("Error: Invalid hexadecimal number.");
        return -1;
    }
    *pDecimal = strtol(hexadecimalStr, NULL, 16);
    return 0;
}

int ColorPrintf(unsigned short color, const char* format, ...){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    unsigned short currentAttr;
    if(GetConsoleScreenBufferInfo(hConsole, &csbi)){
        currentAttr = csbi.wAttributes;
    }else{
        puts("ERROR: Failed to get console screen buffer information.");
        currentAttr = 0x07;
    }
    SetConsoleTextAttribute(hConsole, color);
    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);
    SetConsoleTextAttribute(hConsole, currentAttr); // 恢复原属性
    return ret;
}

int ColorPuts(unsigned short color, const char* Str, unsigned short line){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    unsigned short currentAttr;
    if(GetConsoleScreenBufferInfo(hConsole, &csbi)){
        currentAttr = csbi.wAttributes;
    }else{
        puts("ERROR: Failed to get console screen buffer information.");
        currentAttr = 0x07;
    }
    SetConsoleTextAttribute(hConsole, color);
    PutStr(Str);
    SetConsoleTextAttribute(hConsole, currentAttr); // 恢复原属性
    while(line--){
        putchar('\n');
    }
    return 0;
}

int ChColor(unsigned short color, unsigned short* pOldColor){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(pOldColor!=NULL){
        if(GetConsoleScreenBufferInfo(hConsole, &csbi)){
            *pOldColor = csbi.wAttributes;
        }else{
            puts("ERROR: Failed to get console screen buffer information.");
            *pOldColor = 0x07;
        }
    }
    SetConsoleTextAttribute(hConsole, color);
    return 0;
}

int ChCP(unsigned int newInputCP, unsigned int newOutputCP){
#ifdef _WIN32
    static unsigned int oldInputCP;
    static unsigned int oldOutputCP;
    if(newInputCP != 0){
        oldInputCP = GetConsoleCP();
        SetConsoleCP(newInputCP);
    }else{
        SetConsoleCP(oldInputCP);
    }

    if(newOutputCP != 0){
        oldOutputCP = GetConsoleOutputCP();
        SetConsoleOutputCP(newOutputCP);
    }else{
        SetConsoleOutputCP(oldOutputCP);
    }
#endif
    return 0;
}

// 比较两个字符串是否相同，忽略大小写
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

void PutStr(const char* Str){ // puts()末尾自动换行，printf()不想使用
    for(int i=0;;++i){
        if(*(Str+i)!='\0')
            putchar(*(Str+i));
        else
            break;
    }
}
