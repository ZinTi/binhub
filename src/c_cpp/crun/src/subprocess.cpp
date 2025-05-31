#include "subprocess.h"
#include <bemapiset.h>
#include <cstdio>
#include <iostream>
#include <windows.h>

#include "ProjectCommon.h"
#include "exception.h"
#define BoundaryColor		            0x09

ChildProcess::ChildProcess(const char* newSubproName, const char* newSubproArgs, const bool newVerbose) {
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    this->subproName = new char[strlen(newSubproName) + 1];
    strcpy(this->subproName, newSubproName);
    this->subproArgs = new char[strlen(newSubproArgs) + 1];
    strcpy(this->subproArgs, newSubproArgs);
    this->verbose = newVerbose;
    if (newSubproName==nullptr) {
        PrintError("ChildProcess", "The function parameter \"newSubproName\" must not be NULL!");
        exit(-1);
    }
    if (newSubproArgs != nullptr) {
        this->cmdLine = new char[strlen(newSubproName) + strlen(newSubproArgs) + 3];
        sprintf(this->cmdLine, ".\\%s %s", newSubproName, newSubproArgs);
    }
    else {
        this->cmdLine = new char[strlen(newSubproName) + 3];
        sprintf(this->cmdLine, ".\\%s", newSubproName); // 不能少“.\”，否则将优先运行环境变量能找到的程序，不用“./”的原因是cmd不兼容“./”（powershell兼容）
    }
}

ChildProcess::~ChildProcess() {
    // 确保关闭进程和线程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    delete this->subproName;
    delete this->subproArgs;
    delete this->cmdLine;
}
bool ChildProcess::start() {
    BOOL result = CreateProcessA(
        nullptr,   // 应用程序名称
        cmdLine, // 命令行
        nullptr,   // 进程安全属性
        nullptr,   // 线程安全属性
        FALSE,  // 是否继承句柄
        0,      // 创建标志
        nullptr,   // 环境变量
        nullptr,   // 当前目录
        &si,    // 启动信息
        &pi     // 进程信息
    );
    if (!result) {
        std::cerr << "Failed to create process. Error: " << GetLastError() << std::endl;
        return false;
    }
    return true;
}

void ChildProcess::waitForExit() {
    WaitForSingleObject(pi.hProcess, INFINITE);
}

DWORD ChildProcess::getExitCode() {
    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
        return exitCode;
    }
    else {
        std::cerr << "Failed to get exit code. Error: " << GetLastError() << std::endl;
        return -1;
    }
}

bool ChildProcess::execSubprog() {
    if (this->verbose) {
        crun::ProjectCommon::ColorPrintf(BoundaryColor, "Subprocess: %s\n", this->subproName);
        if (this->subproArgs != nullptr) {
            crun::ProjectCommon::ColorPrintf(BoundaryColor, "Arguments: %s\n", this->subproArgs);
        }
        crun::ProjectCommon::ColorPuts(BoundaryColor, "══════════════════════", 1);
    }
    if (ChildProcess::start()) {
        ChildProcess::waitForExit();
        DWORD exitCode = ChildProcess::getExitCode();
        if (this->verbose) {
            crun::ProjectCommon::ColorPuts(BoundaryColor, "══════════════════════", 1);
            crun::ProjectCommon::ColorPrintf(BoundaryColor, "PID: %d | Return: %d\n", pi.dwProcessId, exitCode);
        }
    }
    else {
        if (this->verbose) {
            crun::ProjectCommon::ColorPuts(BoundaryColor, "══════════════════════", 1);
        }
        std::cerr << "Failed to start child process." << std::endl;
        return false;
    }
    return true;
}