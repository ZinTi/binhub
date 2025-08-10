#include "subprocess.h"
#include <bemapiset.h>
#include <cstdio>
#include <iostream>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

#include "proj_info.h"

namespace cpro{

const binhub_tcolor_t color_boundary = {CLR_BRT_BLUE, CLR_NONE};

Subprocess::Subprocess(const std::string& subprocess_name, const std::string& subprocess_args, bool verbose) {
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    this->subprocess_name_ = subprocess_name;
    this->subprocess_args_ = subprocess_args;
    this->verbose_ = verbose;
    if (this->subprocess_name_.empty()) {
        binhub_cerr("Child process filename must not be empty!");
        exit(-1);
    }
    if (!this->subprocess_args_.empty()) {
        this->cmd_line_ = ".\\" + this->subprocess_name_ + " " + this->subprocess_args_;
    } else {
        this->cmd_line_ = ".\\" + this->subprocess_name_; // 不能少“.\”，否则将优先运行环境变量能找到的程序，不用“./”的原因是cmd不兼容“./”（powershell兼容）
    }
}

Subprocess::~Subprocess(void) {
    // 确保关闭进程和线程句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool Subprocess::start(void) {
    BOOL result = CreateProcessA(
        nullptr,   // 应用程序名称
        &cmd_line_[0], // 命令行
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

void Subprocess::wait_for_exit(void) {
    WaitForSingleObject(pi.hProcess, INFINITE);
}

DWORD Subprocess::get_exit_code(void) {
    DWORD exit_code;
    if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
        return exit_code;
    }
    else {
        std::cerr << "Failed to get exit code. Error: " << GetLastError() << std::endl;
        return -1;
    }
}

bool Subprocess::exec() {
    if (this->verbose_) {
        binhub_cprintf(color_boundary, "Subprocess: %s\n", this->subprocess_name_.c_str());
        if (!this->subprocess_args_.empty()) {
            binhub_cprintf(color_boundary, "Arguments: %s\n", this->subprocess_args_.c_str());
        }
        binhub_cprintf(color_boundary, "══════════════════════"); BINHUB_ENDLINE(1);
    }
    if (Subprocess::start()) {
        Subprocess::wait_for_exit();
        DWORD exit_code = Subprocess::get_exit_code();
        if (this->verbose_) {
            binhub_cprintf(color_boundary, "══════════════════════"); BINHUB_ENDLINE(1);
            binhub_cprintf(color_boundary, "PID: %d | Return: %d\n", pi.dwProcessId, exit_code);
        }
    } else {
        if (this->verbose_) {
            binhub_cprintf(color_boundary, "══════════════════════"); BINHUB_ENDLINE(1);
        }
        binhub_cerr("Failed to start child process."); BINHUB_ENDLINE(1);
        return false;
    }
    return true;
}

} // namespace cpro
