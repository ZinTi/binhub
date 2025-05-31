#include <windows.h>
#include <stdio.h>
#include <string.h>

void show_help(const char *program_name) {
    printf("Usage: %s <filename> <YYYY/MM/DD-HH:MM>\n", program_name);
    printf("Example: %s myfile.txt 2024/05/18-14:06\n", program_name);
}

int validate_datetime_format(const char *datetime) {
    if (strlen(datetime) != 16) return 0;
    // 检查格式 YYYY/MM/DD-HH:MM 是否正确
    for (int i = 0; i < 16; i++) {
        if ((i >= 0 && i <= 3) || (i >= 5 && i <= 6) || (i >= 8 && i <= 9) ||
            (i >= 11 && i <= 12) || (i >= 14 && i <= 15)) {
            if (datetime[i] < '0' || datetime[i] > '9') return 0;
        }
        else if (i == 4 && datetime[i] != '/') return 0;
        else if (i == 7 && datetime[i] != '/') return 0;
        else if (i == 10 && datetime[i] != '-') return 0;
        else if (i == 13 && datetime[i] != ':') return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        show_help(argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *datetime = argv[2];

    // 验证日期时间格式
    if (!validate_datetime_format(datetime)) {
        printf("Error: Invalid datetime format. Expected format is YYYY/MM/DD-HH:MM\n\n");
        show_help(argv[0]);
        return 1;
    }

    // 解析日期时间输入
    SYSTEMTIME st;
    sscanf(datetime, "%4hu/%2hu/%2hu-%2hu:%2hu",
           &st.wYear, &st.wMonth, &st.wDay,
           &st.wHour, &st.wMinute);
    st.wSecond = 0;  // 假设秒数为0
    st.wMilliseconds = 0;

    // 打开文件
    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    // 将SYSTEMTIME转换为FILETIME
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);

    // 设置文件时间：创建时间、最后修改时间、最后访问时间
    if (!SetFileTime(hFile, &ft, &ft, &ft)) {
        printf("Error: Could not set file time\n");
        CloseHandle(hFile);
        return 1;
    }

    printf("File time of %s successfully updated.\n", filename);
    CloseHandle(hFile);
    return 0;
}