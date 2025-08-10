#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 当前项目信息
#define PROJECT_NAME		"BINHUB SelfDel"
#define PROJECT_VER			"0.0.1-WinNT"
#define PROJECT_YEARS		"2023-2025"
#define AUTHOR				"Mr. ZENG Lai(Zin)"
#define CONTACT 			"Email: vip201@126.com"

void del_dir(const char* path);
void rm_current(void);
void rm_module(void);
void print_version(void);
void print_manual(void);

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
            print_version();
        } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--here")) {
            rm_current();
        } else if (!strcmp(argv[1], "-s") || !strcmp(argv[1], "--self")) {
            rm_module();
        } else if(!strcmp(argv[1], "-?") || !strcmp(argv[1], "--help")){
            print_manual();
        }else {
            printf("Invalid argument: %s\n", argv[1]);
        }
    } else {
        print_version();
        print_manual();
    }

    return 0;
}

void del_dir(const char* path) {
    WIN32_FIND_DATA findFileData;
    char search_path[MAX_PATH];
    sprintf(search_path, "%s\\*", path);

    HANDLE hFind = FindFirstFile(search_path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue; // 忽略当前目录和父目录
        }

        sprintf(search_path, "%s\\%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            del_dir(search_path); // 递归删除子目录
        } else {
            if (DeleteFile(search_path) == 0) { // 删除文件
                perror("DeleteFile");
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (RemoveDirectory(path) == 0) { // 删除空目录
        perror("RemoveDirectory");
    }
}

void rm_current(void){
    char current_dir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_dir);
    del_dir(current_dir); // 递归删除当前目录下的所有文件和目录
    printf("All files and directories in %s have been deleted.\n", current_dir);
}

void rm_module(void){
    char exe_path[MAX_PATH];
    GetModuleFileName(NULL, exe_path, MAX_PATH); // 获取程序路径
    // 删除ModuleFile路径下的所有文件和目录
    char* last_backslash = strrchr(exe_path, '\\');
    if (last_backslash != NULL) {
        *last_backslash = '\0';  // 截取目录路径
        del_dir(exe_path);
        printf("All files and directories in %s have been deleted.\n", exe_path);
    } else {
        printf("Unable to determine program directory.\n");
    }
}

void print_version(void){
	printf("%s version %s (compiled %s, %s)\n", PROJECT_NAME, PROJECT_VER, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n%s\n\n", PROJECT_YEARS, AUTHOR, CONTACT);
}

void print_manual(void){
    printf("Usage:\n-v, --version (display version)\n");
    printf("-h, --here (delete current directory)\n");
    printf("-s, --self (delete program directory)\n");
    printf("-?, --help (display the help manual for this program)\n\n");
}