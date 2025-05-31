/***********************************************************
  * 使用Bat脚本也可以
# (1) 远程协助
sc config MpsSvc start= demand
net start MpsSvc
msra.exe /offerra

# (2) 远程桌面
#远程桌面
sc config MpsSvc start= demand
net start MpsSvc
mstsc.exe /v:IPv4 /username

# (3)使用完整的Bat脚本
@echo off

REM 1、启用防火墙

:: MpsSvc服务设置为手动
Set-Service -Name MpsSvc -StartupType Manual
:: 启动MpsSvc服务
net start MpsSvc

REM 2、启用远程桌面服务

:: 设置远程桌面属性
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f
:: 启用远程桌面服务
net start TermService
:: 输出完成消息
echo 远程桌面设置已更新。
:: 暂停批处理脚本，以便查看输出结果
pause

REM 3、修改当前用户

:: 1. 将当前计算机的管理员账户的用户名设置为：“远程使用中，非必要请勿关机”
$userName = "Administrator"
$description = "远程使用中，非必要请勿关机"
net user $userName /fullname:"$description"

:: 2. 修改当前管理员账户的密码，密码从缓冲区接收，由用户输入
$adminPassword = Read-Host "请输入新的管理员密码" -AsSecureString
net user $userName $adminPassword

:: 3. 为账户选择一个新图片：图片路径为“D:\\win7remote.png”
$userImagePath = "D:\win7remote.png"
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\AccountPicture\Users\$userName" /v ImagePath /t REG_EXPAND_SZ /d $userImagePath /f

  * 
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define ADMININFO       "admin_info.txt"
#define ICON_WIN        "win7remote.png"

int StartWindowsFirewall(void);
int StartRemoteDesktopServices(void);
int ChangeAccountNameAndPassword(char* filename, char* userImagePath);
int Whoami(char* filename);
void ReplaceSpecialChars(const char* buffer, char* ipfilename);
void AppendCurrentTimeToFile(char* filename);
void GetAndSaveIPConfig(void);

char ipfilename[256];

int main(int argc, char* argv[]) {
    SetConsoleCP(65001) && SetConsoleOutputCP(65001);
    do{
        printf("请在具有还原卡功能的计算机上使用本程序，并使用管理员权限运行当前程序\n是否继续(Y/n) > ");
        char op = getchar();
        while( getchar() != '\n' );
        if(op=='Y' || op=='y'){
            int r1 = StartWindowsFirewall();
            if(r1 == 1 || r1 == 2){
                int r2 = StartRemoteDesktopServices();
                if(r2 == 1 || r2 == 2){
                    ChangeAccountNameAndPassword(ADMININFO, ICON_WIN);
                }
            }else{
                return -1;
            }

            Whoami(ADMININFO);
            AppendCurrentTimeToFile(ADMININFO);
            GetAndSaveIPConfig();

            system("PAUSE");
            return 0;
        }else if(op=='N' || op=='n'){
            exit(1);
        }else{
            printf("不合法选项\n");
        }
    }while(1);
}


int StartWindowsFirewall(void){
    int r;
    char* command1 = "sc config MpsSvc start= demand";//#MpsSvc服务设置为手动
    char* command2 = "net start MpsSvc";//#启动MpsSvc服务

    if ( 0 == system(command1) ) {
        printf("MpsSvc service start type set successfully!\n");
        int result2 = system(command2);
        if (result2 == 0) {
            r = 1;
            printf("Windows Firewall started successfully!\n");
        }else if(result2 == 2){
            r = 2;
            printf("Windows Firewall does not need to be started again!\n");
        }else{
            r = 3;
            printf("result2 = %d\n",result2);
            printf("Windows Firewall started failed!\n");
        }
    } else {
        r = 4;
        printf("Failed to set the startup type of the MpsSvc service!\n");
    }
    return r;
}

int StartRemoteDesktopServices(void){
    int r;
    char* command1 = "reg add \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\" /v fDenyTSConnections /t REG_DWORD /d 0 /f";//REM 设置远程桌面属性
    char* command2 = "net start TermService";//REM 启用远程桌面服务

    int result1 = system(command1);
    if (result1 == 0) {
        int result2 = system(command2);
        if (result2 == 0) {
            r = 1;
            printf("远程桌面设置已更新！\n");
        }else if(result2 == 2){
            r = 2;
            printf("远程桌面设置无需更新！\n");
        }else{
            r = 3;
            printf("启用远程桌面服务失败!\n");
        }
    } else {
        r = 4;
        printf("设置远程桌面属性失败!\n");
    }
    return r;
}

int ChangeAccountNameAndPassword(char* filename, char* userImagePath) {
    char userInfo[256] = "Password: ";

    char* userName = "Administrator";// 1. 设置管理员账户的用户名和描述
    char* description = "远程使用中，非必要请勿关机";
    char command[256];

    sprintf(command, "net user %s /fullname:\"%s\"", userName, description);
    system(command);

    char adminPassword[100];// 2. 修改管理员账户的密码
    printf("请设置密码：");
    scanf("%s", adminPassword);
    sprintf(command, "net user %s %s", userName, adminPassword);
    system(command);
    FILE *file = fopen(filename,"a");
    if(file == NULL){
        printf("Unable to create/open the file!\n");
    }

    strcat(userInfo,adminPassword);
    fprintf(file,"\n%s",userInfo);
    fclose(file);

    // 3. 为账户选择一个新图片
    sprintf(command, "reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AccountPicture\\Users\\%s\" /v ImagePath /t REG_EXPAND_SZ /d \"%s\" /f", userName, userImagePath);
    system(command);

    return 0;
}

int Whoami(char* filename) {
    FILE *pipe = popen("whoami", "r");// 使用 popen 执行 whoami 命令
    if (pipe == NULL) {
        printf("执行 whoami 命令失败\n");
        return 1;
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        FILE* file = fopen(filename, "a");//追加模式写入
        if (file != NULL) {
            fprintf(file, "\nCurrentUser: %s", buffer);
            fclose(file);
            printf("用户名已写入 %s 文件\n",filename);
        } else {
            printf("无法打开 %s 文件\n",filename);
        }
    } else {
        printf("无法获取当前用户\n");
    }

    pclose(pipe);

    ReplaceSpecialChars(buffer, ipfilename);
    strcat(ipfilename,"IPConfig.txt");

    return 0;
}

void ReplaceSpecialChars(const char* buffer, char* ipfilename) {
    int len = strlen(buffer);

    for (int i = 0; i < len; i++) {
        char currentChar = buffer[i];

        switch (currentChar) {// 检查特定符号并替换
            case '/':
            case '\\':
            case ':':
            case '*':
            case '?':
            case '"':
            case '<':
            case '>':
            case '|':
            case '\n':
            case '\0':
                ipfilename[i] = '_';
                break;
            default:
                ipfilename[i] = currentChar;
        }
    }
    ipfilename[len] = '\0'; // 添加字符串终止符
}

void GetAndSaveIPConfig(void) {
    FILE *fp;
    char command[100];

    if ((fp = fopen(ipfilename, "r")) == NULL) {
        // 如果文件不存在，创建它
        fp = fopen(ipfilename, "w");
        if (fp == NULL) {
            printf("无法创建文件 %s \n", ipfilename);
            return;
        }
        fclose(fp);
    }

    // 使用系统命令获取IP配置，并将结果写入文件
    strcpy(command, "ipconfig > ");
    strcat(command, ipfilename);
    system(command);

    printf("IP配置已写入 %s 文件\n", ipfilename);
}

void AppendCurrentTimeToFile(char* filename) {
    FILE* fp = fopen(filename, "a");
    if (fp != NULL){
        time_t currentTime;
        struct tm* timeInfo;
        char timeString[80];

        time(&currentTime);
        timeInfo = localtime(&currentTime);
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);

        fprintf(fp, "CurrentTime: %s\n", timeString);
        fclose(fp);
        printf("当前时间已写入 %s 文件\n", filename);
    }else{
        printf("无法打开 %s 文件\n", filename);
    }
}
