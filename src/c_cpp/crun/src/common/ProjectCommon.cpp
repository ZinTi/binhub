#include "ProjectCommon.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <cstdarg>
#include <ctime>

#define PROJ_ENV_VAR      "BINHUB_HOME"

namespace crun {
    std::string ProjectCommon::checkEnv() {
        if(getenv(PROJ_ENV_VAR) == nullptr){
            puts("Project is not installed!");
            return "";
        }
        return getenv(PROJ_ENV_VAR);
    }

    void ProjectCommon::ReadManual(const char* path){
	    ;
    }


    // HEX -> DEC
    int ProjectCommon::ConvertHex(char* hexadecimalStr, long* pDecimal){
        char* endptr;
        strtol(hexadecimalStr, &endptr, 16);
        if (*endptr != '\0') {
		    puts("Error: Invalid hexadecimal number.");
            return -1;
        }
        *pDecimal = strtol(hexadecimalStr, NULL, 16);
	    return 0;
    }

    int ProjectCommon::PutStr(const char* Str){ // puts()自动换行，printf()不想使用，故写了这个函数
	    for(int i=0;;++i){
		    if(*(Str+i)!='\0')
			    putchar(*(Str+i));
		    else
			    break;
	    }
	    return 0;
    }

    int ProjectCommon::ColorPrintf(const unsigned short color, const char* format, ...){
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

    int ProjectCommon::ColorPuts(const unsigned short color, const char* Str, unsigned short line){
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

    int ProjectCommon::ChColor(const unsigned short color, unsigned short* pOldColor){
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

    int ProjectCommon::ChCP(const unsigned int newInputCP, unsigned int newOutputCP){
        static unsigned int oldInputCP;
        static unsigned int oldOutputCP;
	    if(newInputCP != 0){ //
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
	    return 0;
    }

    // UTF-8 检测函数
    int ProjectCommon::IsUTF8(const char* data, const int len) {
        int i = 0;
        while (i < len) {
            if ((data[i] & 0x80) == 0) {  // 0xxxxxxx
                i++;
            } else if ((i + 1 < len) && ((data[i] & 0xE0) == 0xC0) && ((data[i + 1] & 0xC0) == 0x80)) {  // 110xxxxx 10xxxxxx
                i += 2;
            } else if ((i + 2 < len) && ((data[i] & 0xF0) == 0xE0) && ((data[i + 1] & 0xC0) == 0x80) && ((data[i + 2] & 0xC0) == 0x80)) {  // 1110xxxx 10xxxxxx 10xxxxxx
                i += 3;
            } else {
                return 0;  // 不是UTF-8
            }
        }
        return 1;  // 是UTF-8
    }

    // 自动选择编码页函数
    int ProjectCommon::AutomaticallySelectCodePage(const char* filename) {
        FILE* inputFile = fopen(filename, "rb");
        if (!inputFile) {
            ColorPuts(0x04, "Failed to open input file!", 1);
            exit(1);
        }

        // 获取文件大小
        fseek(inputFile, 0, SEEK_END);
        long fileSize = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        // 分配内存读取文件内容
        char* data = (char*)malloc(fileSize);
        if (!data) {
            fclose(inputFile);
            ColorPuts(0x04, "Memory allocation failed!", 1);
            exit(1);
        }
        fread(data, 1, fileSize, inputFile);
        fclose(inputFile);

        // 检测是否为UTF-8
        int isUTF8 = IsUTF8(data, fileSize);
        free(data);

        if (isUTF8) {
            if (GetConsoleCP() != 65001) {
                SetConsoleCP(65001);
            }
            if (GetConsoleOutputCP() != 65001) {
                SetConsoleOutputCP(65001);
            }
            ColorPuts(0xD0, "Set CodePage: CP65001/UTF-8", 1);
        } else {
            if (GetConsoleCP() != 936) {
                SetConsoleCP(936);
            }
            if (GetConsoleOutputCP() != 936) {
                SetConsoleOutputCP(936);
            }
            ColorPuts(0xD0, "Set CodePage: CP936/GB2312", 1);
        }
        return 0;
    }

    // 将字符串中的所有字母转换成大写
    void ProjectCommon::StrToUppercase(char *str) {
        while (*str) {
            if (*str >= 'a' && *str <= 'z') {
                *str = *str - 'a' + 'A';
            }
            str++;
        }
    }

    // 将字符串中的所有字母转换成小写
    void ProjectCommon::StrToLowercase(char *str) {
        while (*str) {
            if (*str >= 'A' && *str <= 'Z') {
                *str = *str - 'A' + 'a';
            }
            str++;
        }
    }

    // 比较两个字符串是否相同，忽略大小写
    int ProjectCommon::StrCmpIgnoreCase(const char *str1, const char *str2) {
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

    // 比较两个字符串的前n个字符是否相同，忽略大小写
    int ProjectCommon::StrNCmpIgnoreCase(const char *str1, const char *str2, unsigned long long n) {
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

    void ProjectCommon::LoadLoop(const char* text, int style){
        char* sym_braille[9] = {"⣷","⣯","⣟","⡿","⢿","⣻","⣽","⣾","⣿"};
        char* sym_ascii[4] = {"-","\\","|","/"};
        typedef struct {
            char** sym; // 字符数组指针
            int num;    // 字符数
            int sleep;  // 字符速度（休眠时间）
        } LoadLoopAttr;
        LoadLoopAttr styleAttr1 = {sym_braille, 9, 60};
        LoadLoopAttr styleAttr2 = {sym_ascii, 4, 180};

        unsigned int initialInputCP, initialOutputCP;
        initialInputCP = GetConsoleCP();
        initialOutputCP = GetConsoleOutputCP();
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // 获取标准输出句柄
        CONSOLE_CURSOR_INFO cursorInfo;                // 光标信息变量
        GetConsoleCursorInfo(hOut, &cursorInfo);       // 获取当前光标信息
        cursorInfo.dwSize = 25;
        cursorInfo.bVisible = 0;                       // 隐藏光标
        SetConsoleCursorInfo(hOut, &cursorInfo);

        // 显示Load Loop
        int sleepnum = (style==0 ? styleAttr1.sleep : styleAttr2.sleep);
        for(int i = 0 ; ; i++){
            i %= (style==0 ? styleAttr1.num : styleAttr2.num);                            // 循环符号数组
            printf("\r%s %s", (style==0 ? styleAttr1.sym[i] : styleAttr2.sym[i]), text);     // 使用符号输出进度
            Sleep(sleepnum);                        // 休眠
        }

        cursorInfo.bVisible = 1;                       // 重新显示光标
        SetConsoleCursorInfo(hOut, &cursorInfo);
        SetConsoleCP(initialInputCP);
        SetConsoleOutputCP(initialOutputCP);
    }
} // crun