#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

using namespace std;

#define PROJECT_NAME            "BINHUB SetFileEnc"
#define PROJECT_VERSION         "0.0.1-WinNT"
#define PROJECT_YEARS           "2024-2025"
#define AUTHOR                  "Mr. ZENG Lai(Zin)"
#define CONTACT                 "vip201@126.com"


binhub_tcolor_t CLR_PROMPT_OK            = {CLR_BRT_GREEN, CLR_NONE};
binhub_tcolor_t CLR_PROMPT_ERROR         = {CLR_RED, CLR_NONE};
binhub_tcolor_t CLR_PROMPT_WARN          = {CLR_BRT_RED, CLR_NONE};
binhub_tcolor_t CLR_PROMPT_AWAIT         = {CLR_BRT_CYAN, CLR_NONE};
binhub_tcolor_t CLR_PROMPT_DEFAULT       = {CLR_WHITE, CLR_NONE};
binhub_tcolor_t CLR_DELIMITER            = {CLR_BRT_MAGENTA, CLR_NONE};


// 函数声明
void show_version(void);
void show_manual(char* prog_name);
bool is_utf8(const vector<char>& data);
string gbk_to_utf8(const string& gbk_str);
string utf8_to_gbk(const string& utf8_str);

int main(int argc, char* argv[]) {
    // 根据参数选择程序功能
    if(argc>4){
        show_version();
        binhub_cprintf(CLR_PROMPT_ERROR, "Too many arguments!"); BINHUB_ENDLINE(1);
        show_manual(argv[0]);
    } else if(argc==4||argc==3){ // 提供文件编码和模式
        if(!strncmp(argv[1], "--to=", 5)){
            ifstream input_file(argv[2], ios::binary);
            if (!input_file.is_open()) {
                binhub_cprintf(CLR_PROMPT_ERROR, "Failed to open input file!"); BINHUB_ENDLINE(1);
                return 1;
            }

            vector<char> data((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
            input_file.close();

            bool isUTF8 = is_utf8(data);
            string output_str;

            if(!strcmp(argv[1]+5, "gb2312")||!strcmp(argv[1]+5, "GB2312")||!strcmp(argv[1]+5, "gbk")||!strcmp(argv[1]+5, " GBK ")||!strcmp(argv[1]+5, "ANSI")){
                if (!isUTF8) {
                    binhub_cprintf(CLR_PROMPT_WARN, "Input file is not UTF-8 encoded."); BINHUB_ENDLINE(1);
                    return 1;
                }
                output_str = utf8_to_gbk(string(data.begin(), data.end()));

            } else if(!strcmp(argv[1]+5, "utf8")||!strcmp(argv[1]+5, "UTF8")||!strcmp(argv[1]+5, "utf-8")||!strcmp(argv[1]+5, " UTF-8 ")){
                if (isUTF8) {
                    binhub_cprintf(CLR_PROMPT_WARN, "Input file is not  GBK /GB2312 encoded."); BINHUB_ENDLINE(1);
                    return 1;
                }
                output_str = gbk_to_utf8(string(data.begin(), data.end()));

            } else {
                binhub_cprintf(CLR_PROMPT_WARN, "This encoding format is not supported: %s", argv[1]+5); BINHUB_ENDLINE(1);
                return 1;
            }

            if(argc==4){
                ofstream output_file(argv[3], ios::binary);
                if (!output_file.is_open()) {
                    binhub_cprintf(CLR_PROMPT_ERROR, "Failed to open output file!"); BINHUB_ENDLINE(1);
                    return 1;
                }
                output_file.write(output_str.c_str(), output_str.size());
                output_file.close();
                binhub_cprintf(CLR_PROMPT_OK, "Conversion completed: %s", argv[3]); BINHUB_ENDLINE(1);
            } else {
                char* backupFileName = (char*)malloc(PATH_MAX);
                sprintf(backupFileName, "%s.bak", argv[2]);
                if(CopyFileA(argv[2], backupFileName, false)==0){
                    binhub_cprintf(CLR_PROMPT_WARN, "Failed to back up the file: %s", argv[2]); BINHUB_ENDLINE(1);
                    return 1;
                }

                free(backupFileName);
                ofstream output_file(argv[2], ios::binary); // 不改名输出
                if (!output_file.is_open()) {
                    binhub_cprintf(CLR_PROMPT_ERROR, "Failed to open output file!"); BINHUB_ENDLINE(1);
                    return 1;
                }
                output_file.write(output_str.c_str(), output_str.size());
                output_file.close();
                binhub_cprintf(CLR_PROMPT_OK, "Conversion completed: %s", argv[2]); BINHUB_ENDLINE(1);
            }
        } else {
            binhub_cprintf(CLR_PROMPT_ERROR, "Unknown operation: %s", argv[1]); BINHUB_ENDLINE(1);
        }
    } else if(argc==2){ // 只提供文件名，判断文件编
        ifstream input_file(argv[1], ios::binary);
        if (!input_file.is_open()) {
            binhub_cprintf(CLR_PROMPT_ERROR, "Failed to open input file!"); BINHUB_ENDLINE(1);
            return 1;
        }

        vector<char> data((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        input_file.close();

        bool isUTF8 = is_utf8(data);
        string output_str;
        binhub_cprintf(CLR_DELIMITER, "The %s file is %s encoded.", argv[1], (isUTF8 ? " UTF-8 " : " GBK /GB2312")); BINHUB_ENDLINE(1);
    } else {
        show_version();
        binhub_cprintf(CLR_PROMPT_ERROR, "Please provide arguments!"); BINHUB_ENDLINE(1);
        show_manual(argv[0]);
    }

    return 0;
}

//函数原型

void show_version(void){
    const std::string info = PROJECT_NAME " version " PROJECT_VERSION " (compiled " __TIME__ ", " __DATE__ ")\n"
    "Copyleft (c) " PROJECT_YEARS " " AUTHOR ". All rights reserved. " CONTACT "\n";
    std::cout << info;
}

void show_manual(char* prog_name){
    //std::cerr << "Usage: " << prog_name << " <input file> <output file> [u-to-g|g-to-u]" << std::endl;
    std::cout << "Usage:\n\t" << prog_name << " [ --to=gbk | --to=utf-8 ] <input file> [output file]" << std::endl;
    std::cout << "If multiple encodings such as GBK and UTF-8 are present in the text, only one will be recognized." << std::endl;
}

// 判断文件是否为 UTF-8 编码
bool is_utf8(const vector<char>& data) {
    int len = data.size();
    int i = 0;
    while (i < len) {
        if ((data[i] & 0x80) == 0) { // 0xxxxxxx
            i++;
        } else if ((i + 1 < len) && ((data[i] & 0xE0) == 0xC0) && ((data[i + 1] & 0xC0) == 0x80)) { // 110xxxxx 10xxxxxx
            i += 2;
        } else if ((i + 2 < len) && ((data[i] & 0xF0) == 0xE0) && ((data[i + 1] & 0xC0) == 0x80) && ((data[i + 2] & 0xC0) == 0x80)) { // 1110xxxx 10xxxxxx 10xxxxxx
            i += 3;
        } else {
            return false;
        }
    }
    return true;
}

// 将 GBK 编码转换为 UTF-8 
string gbk_to_utf8(const string& gbk_str) {
    int len = MultiByteToWideChar(CP_ACP, 0, gbk_str.c_str(), -1, NULL, 0);
    vector<wchar_t> wide_str(len);
    MultiByteToWideChar(CP_ACP, 0, gbk_str.c_str(), -1, &wide_str[0], len);

    len = WideCharToMultiByte(CP_UTF8, 0, &wide_str[0], -1, NULL, 0, NULL, NULL);
    vector<char> utf8_str(len);
    WideCharToMultiByte(CP_UTF8, 0, &wide_str[0], -1, &utf8_str[0], len, NULL, NULL);

    return string(&utf8_str[0]);
}

// 将 UTF-8 编码转换为 GBK 
string utf8_to_gbk(const string& utf8_str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, NULL, 0);
    vector<wchar_t> wide_str(len);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], len);

    len = WideCharToMultiByte(CP_ACP, 0, &wide_str[0], -1, NULL, 0, NULL, NULL);
    vector<char> gbk_str(len);
    WideCharToMultiByte(CP_ACP, 0, &wide_str[0], -1, &gbk_str[0], len, NULL, NULL);

    return string(&gbk_str[0]);
}
