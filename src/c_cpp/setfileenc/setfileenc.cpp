#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
using namespace std;

#define PROJECT_NAME            "SetFileEnc"
#define PROJECT_VERSION         "0.1-WinNT"
#define PROJECT_YEARS           "2024-2025"
#define AUTHOR                  "Mr. ZENG Lai(Zin)"
#define CONTACT                 "vip201@126.com"


#define COLOR_PROMPT_OK			0x0A
#define COLOR_PROMPT_ERROR		0x04
#define COLOR_PROMPT_WARN		0x0C
#define COLOR_PROMPT_AWAIT		0x0B
#define COLOR_PROMPT_DEFAULT	0x07
#define COLOR_DELIMITER			0x0D


// 函数声明
void ShowVersion(void);
void ShowManual(char* progName);
bool IsUTF8(const vector<char>& data);
string GBKToUTF8(const string& gbkStr);
string UTF8ToGBK(const string& utf8Str);

void PutColorfulString(const char* Str, WORD color, int line);
void ChangeConsoleTextColor(WORD color);
void ChangeCodePage(UINT inputCP, UINT outputCP);

int main(int argc, char* argv[]) {
    // 根据参数选择程序功能
    if(argc>4){
        ShowVersion();
        PutColorfulString("Too many arguments!", COLOR_PROMPT_ERROR, 1);
        ShowManual(argv[0]);
    }else if(argc==4||argc==3){ // 提供文件编码和模式
        if(!strncmp(argv[1], "--to=", 5)){
            ifstream inputFile(argv[2], ios::binary);
            if (!inputFile.is_open()) {
                PutColorfulString("Failed to open input file!", COLOR_PROMPT_ERROR, 1);
                return 1;
            }

            vector<char> data((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
            inputFile.close();

            bool isUTF8 = IsUTF8(data);
            string outputStr;

            if(!strcmp(argv[1]+5, "gb2312")||!strcmp(argv[1]+5, "GB2312")||!strcmp(argv[1]+5, "gbk")||!strcmp(argv[1]+5, "GBK")||!strcmp(argv[1]+5, "ANSI")){
                if (!isUTF8) {
                    PutColorfulString("Input file is not UTF-8 encoded.", COLOR_PROMPT_WARN, 1);
                    return 1;
                }
                outputStr = UTF8ToGBK(string(data.begin(), data.end()));

            }else if(!strcmp(argv[1]+5, "utf8")||!strcmp(argv[1]+5, "UTF8")||!strcmp(argv[1]+5, "utf-8")||!strcmp(argv[1]+5, "UTF-8")){
                if (isUTF8) {
                    PutColorfulString("Input file is not GBK/GB2312 encoded.", COLOR_PROMPT_WARN, 1);
                    return 1;
                }
                outputStr = GBKToUTF8(string(data.begin(), data.end()));

            }else{
                ChangeConsoleTextColor(COLOR_PROMPT_WARN);
                std::cout << "This encoding format is not supported: " << argv[1]+5 << std::endl;
                ChangeConsoleTextColor(COLOR_PROMPT_DEFAULT);
                return 1;
            }

            if(argc==4){
                ofstream outputFile(argv[3], ios::binary);
                if (!outputFile.is_open()) {
                    PutColorfulString("Failed to open output file!", COLOR_PROMPT_ERROR, 1);
                    return 1;
                }
                outputFile.write(outputStr.c_str(), outputStr.size());
                outputFile.close();
                ChangeConsoleTextColor(COLOR_PROMPT_OK);
                std::cout << "Conversion completed: " << argv[3] << std::endl;
                ChangeConsoleTextColor(COLOR_PROMPT_DEFAULT);
            }else{
                char* backupFileName = (char*)malloc(PATH_MAX);
                sprintf(backupFileName, "%s.bak", argv[2]);
                if(CopyFileA(argv[2], backupFileName, false)==0){
                    ChangeConsoleTextColor(COLOR_PROMPT_WARN);
                    std::cout << "Failed to back up the file: " << argv[2] << std::endl;
                    ChangeConsoleTextColor(COLOR_PROMPT_DEFAULT);
                    return 1;
                }

                free(backupFileName);
                ofstream outputFile(argv[2], ios::binary); // 不改名输出
                if (!outputFile.is_open()) {
                    PutColorfulString("Failed to open output file!", COLOR_PROMPT_ERROR, 1);
                    return 1;
                }
                outputFile.write(outputStr.c_str(), outputStr.size());
                outputFile.close();
                ChangeConsoleTextColor(COLOR_PROMPT_OK);
                std::cout << "Conversion completed: " << argv[2] << std::endl;
                ChangeConsoleTextColor(COLOR_PROMPT_DEFAULT);
            }

        }else{
            ChangeConsoleTextColor(COLOR_PROMPT_ERROR);
            cerr << "Unknown operation: " << argv[1] << std::endl;
            ChangeConsoleTextColor(COLOR_PROMPT_DEFAULT);
        }
    }else if(argc==2){ // 只提供文件名，判断文件编
        ifstream inputFile(argv[1], ios::binary);
        if (!inputFile.is_open()) {
            PutColorfulString("Failed to open input file!", COLOR_PROMPT_ERROR, 1);
            return 1;
        }

        vector<char> data((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        bool isUTF8 = IsUTF8(data);
        string outputStr;
        ChangeConsoleTextColor(0x0D);
        std::cout << "The " << argv[1] << " file is " << (isUTF8 ? "UTF-8" : "GBK/GB2312") << " encoded." << std::endl;
        ChangeConsoleTextColor(COLOR_PROMPT_DEFAULT);
    }else{
        ShowVersion();
        PutColorfulString("Please provide arguments!", COLOR_PROMPT_ERROR, 1);
        ShowManual(argv[0]);
    }

    return 0;
}

//函数原型

void ShowVersion(void){
    const std::string info = PROJECT_NAME " version " PROJECT_VERSION " (compiled " __TIME__ ", " __DATE__ ")\n"
    "Copyleft (c) " PROJECT_YEARS " " AUTHOR ". All rights reserved. " CONTACT "\n";
    std::cout << info;
}

void ShowManual(char* progName){
    //cerr << "Usage: " << progName << " <input file> <output file> [u-to-g|g-to-u]" << std::endl;
    std::cout << "Usage:\n\t" << progName << " [ --to=gbk | --to=utf-8 ] <input file> [output file]" << std::endl;
    std::cout << "If multiple encodings such as GBK and UTF-8 are present in the text, only one will be recognized." << std::endl;
}

// 判断文件是否为UTF-8编码
bool IsUTF8(const vector<char>& data) {
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

// 将GBK编码转换为UTF-8
string GBKToUTF8(const string& gbkStr) {
    int len = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
    vector<wchar_t> wideStr(len);
    MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wideStr[0], len);

    len = WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], -1, NULL, 0, NULL, NULL);
    vector<char> utf8Str(len);
    WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], -1, &utf8Str[0], len, NULL, NULL);

    return string(&utf8Str[0]);
}

// 将UTF-8编码转换为GBK
string UTF8ToGBK(const string& utf8Str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
    vector<wchar_t> wideStr(len);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], len);

    len = WideCharToMultiByte(CP_ACP, 0, &wideStr[0], -1, NULL, 0, NULL, NULL);
    vector<char> gbkStr(len);
    WideCharToMultiByte(CP_ACP, 0, &wideStr[0], -1, &gbkStr[0], len, NULL, NULL);

    return string(&gbkStr[0]);
}

void PutColorfulString(const char* Str, WORD color, int line){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
    std::cout << Str;
	SetConsoleTextAttribute(hConsole, 0x7);
	for(int i=0;i<line;i++){
		std::cout << std::endl;
	}
}
void ChangeConsoleTextColor(WORD color){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
void ChangeCodePage(UINT inputCP, UINT outputCP){
	//system("chcp `Active code page`");
	SetConsoleCP(inputCP);
	SetConsoleOutputCP(outputCP);
}