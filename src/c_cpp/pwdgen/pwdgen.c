// 随机密码生成器-命令行版
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "binhub.h"

// 当前项目信息
#define PROJ_NAME			"BINHUB PwdGen"
#ifdef _WIN32
    #define PROJ_VER			"0.0.1-WinNT"
#else
    #define PROJ_VER			"0.0.1-GNU/Linux"
#endif
#define PROJ_YEARS			"2024-2025"
#define AUTHOR				"Mr. ZENG Lai(Zin)"
#define CONTACT 			"Email: vip201@126.com"

// 定义密码组成成分
#define DIGITS                 "0123456789"
#define UPPERCASE_LETTERS      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE_LETTERS      "abcdefghijklmnopqrstuvwxyz"
#define SPECIAL_CHARACTERS     "!@#$\x25^&*()_+{}[]|:;<>,.?/~"

binhub_tcolor_t CLR_PROMPT_General    =	{CLR_BRT_WHITE, CLR_NONE};
binhub_tcolor_t CLR_PROMPT_Error      =	{CLR_RED, CLR_NONE};
binhub_tcolor_t CLR_DIGITS            =	{CLR_BRT_GREEN, CLR_NONE};
binhub_tcolor_t CLR_UPPERCASE         =	{CLR_BRT_BLUE, CLR_NONE};
binhub_tcolor_t CLR_LOWERCASE         =	{CLR_BRT_CYAN, CLR_NONE};
binhub_tcolor_t CLR_SPECIAL           =	{CLR_BRT_MAGENTA, CLR_NONE};
binhub_tcolor_t CLR_SerialNumber      = {CLR_BLACK, CLR_BRT_YELLOW};

// 函数声明
void about(void);
void gen_random_passwd(char* password, const char* components, int length);
int cprint_passwd(char* password);
int get_passwd_comp(char* components);
int get_passwd_len(int* pPwdLength);
int get_passwd_count(int* pPwdCount);

int main(void) {
    about();

    while(1){
        char option;
        printf("Begin? (y/N) > ");
        _label:
        option = (char)getchar();
        if(option == '\n' || option == '\r'){
            goto _label;
        }
        if(option == 'y' || option == 'Y'){
            srand(time(NULL)); // 初始化随机数生成器
            int password_length, password_count;
            char* components = (char*)malloc(100);
            
            // 获取密码组成成分、密码长度和密码个数
            get_passwd_comp(components);
            get_passwd_len(&password_length);
            get_passwd_count(&password_count);
            
            // 生成密码
            for (int i = 0; i < password_count; i++) {
                char password[password_length + 1];
                gen_random_passwd(password, components, password_length);
                binhub_cprintf(CLR_SerialNumber, "%02d", i + 1); // 序号
                cprint_passwd(password);
            }
            if(components) free(components);
            printf("End\n");
        }else if(option == 'n' || option == 'N'){
            break;
        }else {
            binhub_cprintf(CLR_PROMPT_Error, "Invalid option! Enter 'y' or 'n', please!\n");
        }
    }

    return 0;
}

void about(void){
	printf("%s version %s (compiled %s, %s)\n", PROJ_NAME, PROJ_VER, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n%s\n\n", PROJ_YEARS, AUTHOR, CONTACT);
}

int cprint_passwd(char* password){
    printf("\t");
    for(int i=0 ; ; i++){
        if(*(password+i) >= '0' && *(password+i) <= '9'){  // 0-9数字
            binhub_cprintf(CLR_DIGITS, "%c",*(password+i));
        }else if(*(password+i) >= 'A' && *(password+i) <= 'Z'){ //A-Z大写字母
            binhub_cprintf(CLR_UPPERCASE, "%c",*(password+i));
        }else if(*(password+i) >= 'a' && *(password+i) <= 'z'){ //a-z小写字母
            binhub_cprintf(CLR_LOWERCASE, "%c",*(password+i));
        }else if(*(password+i) == '\0'){ //字符串结束符
            break;
        }else { //其他字符
            //*(password+i) > '9' && *(password+i) < 'A' || *(password+i) < '0' || *(password+i) > 'Z' && *(password+i) < 'a' || *(password+i) > 'z'
            binhub_cprintf(CLR_SPECIAL, "%c",*(password+i));
        }
    }
    putchar('\n');

    return 0;
}

int get_passwd_comp(char* components){
    binhub_cprintf(CLR_DIGITS, "D/d\tDigits( "DIGITS" )\n");
    binhub_cprintf(CLR_UPPERCASE, "U/u\tUppercase letters( "UPPERCASE_LETTERS" )\n");
    binhub_cprintf(CLR_LOWERCASE, "L/l\tLowercase letters( "LOWERCASE_LETTERS" )\n");
    binhub_cprintf(CLR_SPECIAL, "S/s\tSpecial characters( "SPECIAL_CHARACTERS" )\n");
    binhub_cprintf(CLR_PROMPT_General, "Enter password components > ");
    if(scanf("%s", components)){};
    // 合法性显示
    char* theUnderline = (char*)malloc(sizeof(components));
    for(int i=0 ; ; i++){
        if(*(components+i) == 'D' || *(components+i) == 'd'){
            binhub_cprintf(CLR_DIGITS, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == 'U' || *(components+i) == 'u'){
            binhub_cprintf(CLR_UPPERCASE, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == 'S' || *(components+i) == 's'){
            binhub_cprintf(CLR_SPECIAL, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == 'L' || *(components+i) == 'l'){
            binhub_cprintf(CLR_LOWERCASE, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == '\0'){
            *(theUnderline+i) = '\0';
            break;
        }else {
            binhub_cprintf(CLR_PROMPT_Error, "%c",*(components+i));
            *(theUnderline+i) = '^';
        }
    }
    binhub_cprintf(CLR_PROMPT_Error, "\n%s\n", theUnderline);

    return 0;
}

int get_passwd_len(int* pPwdLength){
    binhub_cprintf(CLR_PROMPT_General, "Enter password length > ");
    if(scanf("%d", pPwdLength)){};
    return 0;
}

int get_passwd_count(int* pPwdCount){
    binhub_cprintf(CLR_PROMPT_General, "Enter number of passwords to generate > ");
    if(scanf("%d", pPwdCount)){};
    return 0;
}

void gen_random_passwd(char* password, const char* components, int length) {
    int components_length = 0;
    int x1[4] = {0,0,0,0};
    if (strchr(components, 'D') != NULL || strchr(components, 'd') != NULL){
        components_length += strlen(DIGITS);
        x1[0] = 1;
    }
    if (strchr(components, 'U') != NULL || strchr(components, 'u') != NULL){
        components_length += strlen(UPPERCASE_LETTERS);
        x1[1] = 1;
    }
    if (strchr(components, 'L') != NULL || strchr(components, 'l') != NULL){
        components_length += strlen(LOWERCASE_LETTERS);
        x1[2] = 1;
    }
    if (strchr(components, 'S') != NULL || strchr(components, 's') != NULL){
        components_length += strlen(SPECIAL_CHARACTERS);
        x1[3] = 1;
    }
    for (int i = 0; i < length; i++) {
        size_t index = rand() % components_length;
        if (x1[0] == 1) {
            if (index < strlen(DIGITS)) {
                password[i] = DIGITS[index];
                continue;
            }
            index -= strlen(DIGITS);
        }
        if (x1[1] == 1) {
            if (index < strlen(UPPERCASE_LETTERS)) {
                password[i] = UPPERCASE_LETTERS[index];
                continue;
            }
            index -= strlen(UPPERCASE_LETTERS);
        }
        if (x1[2] == 1) {
            if (index < strlen(LOWERCASE_LETTERS)) {
                password[i] = LOWERCASE_LETTERS[index];
                continue;
            }
            index -= strlen(LOWERCASE_LETTERS);
        }
        if (x1[3] == 1) {
            if (index < strlen(SPECIAL_CHARACTERS)) {
                password[i] = SPECIAL_CHARACTERS[index];
                continue;
            }
            index -= strlen(SPECIAL_CHARACTERS);
        }
    }
    password[length] = '\0'; // 添加字符串结束符
    return;
}
