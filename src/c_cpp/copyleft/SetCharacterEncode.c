#include "SetCharacterEncode.h"

#ifdef _WIN32
    #include <windows.h>
#endif
#include <stdio.h>
#include "color.h"

int changeCodePage(unsigned int newInputCP, unsigned int newOutputCP){
#ifdef _WIN32
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
#endif
    return 0;
}

int isDataUTF8(const char* data, int len) {
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

int automaticallySelectCodePage(const char* filename) {
    const TextColor failedColor = { COLOR_RED, COLOR_NONE };
    const TextColor successColor = { COLOR_BLACK, COLOR_BRIGHT_MAGENTA };
    FILE* inputFile = fopen(filename, "rb");
    if (!inputFile) {
        ColorPrintf(failedColor, "Failed to open input file!\n");
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
        ColorPrintf(failedColor, "Memory allocation failed!\n");
        exit(1);
    }
    int returnValue = fread(data, 1, fileSize, inputFile);
    fclose(inputFile);

    // 检测是否为UTF-8
    int isUTF8 = isDataUTF8(data, fileSize);
    free(data);

#ifdef _WIN32
    if (isUTF8) {
        if (GetConsoleCP() != 65001) {
            SetConsoleCP(65001);
        }
        if (GetConsoleOutputCP() != 65001) {
            SetConsoleOutputCP(65001);
        }
        ColorPrintf(successColor, "Set CodePage: CP65001/UTF-8\n");
    } else {
        if (GetConsoleCP() != 936) {
            SetConsoleCP(936);
        }
        if (GetConsoleOutputCP() != 936) {
            SetConsoleOutputCP(936);
        }
        ColorPrintf(successColor, "Set CodePage: CP936/GB2312\n");
    }
#else
    if(isUTF8){
        ColorPrintf(successColor, "The Current Charactor Encode of File is UTF-8.\n");
    }else{
        ColorPrintf(failedColor, "The Current Charactor Encode of File is not UTF-8.\n");
    }
#endif
    return 0;
}