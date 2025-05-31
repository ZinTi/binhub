#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 当前项目信息
#define PROJECT_NAME		"SelfDelete"
#define PROJECT_VER			"0.1-WinNT"
#define PROJECT_YEARS		"2023-2025"
#define AUTHOR				"Mr. ZENG Lai(Zin)"
#define CONTACT 			"Email: vip201@126.com"

void DeleteDirectory(const char *path);
void RemoveCurrent(void);
void RemoveModule(void);
void ShowVersion(void);
void ShowManual(void);

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
            ShowVersion();
        } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--here")) {
            RemoveCurrent();
        } else if (!strcmp(argv[1], "-s") || !strcmp(argv[1], "--self")) {
            RemoveModule();
        } else if(!strcmp(argv[1], "-?") || !strcmp(argv[1], "--help")){
            ShowManual();
        }else {
            printf("Invalid argument: %s\n", argv[1]);
        }
    } else {
        ShowVersion();
        ShowManual();
    }

    return 0;
}

void DeleteDirectory(const char *path) {
    WIN32_FIND_DATA findFileData;
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\*", path);

    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue; // 忽略当前目录和父目录
        }

        sprintf(searchPath, "%s\\%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            DeleteDirectory(searchPath); // 递归删除子目录
        } else {
            if (DeleteFile(searchPath) == 0) { // 删除文件
                perror("DeleteFile");
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (RemoveDirectory(path) == 0) { // 删除空目录
        perror("RemoveDirectory");
    }
}

void RemoveCurrent(void){
    char currentDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDirectory);
    DeleteDirectory(currentDirectory); // 递归删除当前目录下的所有文件和目录
    printf("All files and directories in %s have been deleted.\n", currentDirectory);
}

void RemoveModule(void){
    char executablePath[MAX_PATH];
    GetModuleFileName(NULL, executablePath, MAX_PATH); // 获取程序路径
    // 删除ModuleFile路径下的所有文件和目录
    char *lastBackslash = strrchr(executablePath, '\\');
    if (lastBackslash != NULL) {
        *lastBackslash = '\0';  // 截取目录路径
        DeleteDirectory(executablePath);
        printf("All files and directories in %s have been deleted.\n", executablePath);
    } else {
        printf("Unable to determine program directory.\n");
    }
}

void ShowVersion(void){
	printf("%s version %s (compiled %s, %s)\n", PROJECT_NAME, PROJECT_VER, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n%s\n\n", PROJECT_YEARS, AUTHOR, CONTACT);
}

void ShowManual(void){
    printf("Usage:\n-v, --version (display version)\n");
    printf("-h, --here (delete current directory)\n");
    printf("-s, --self (delete program directory)\n");
    printf("-?, --help (display the help manual for this program)\n\n");
}