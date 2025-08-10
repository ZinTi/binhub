#include "memory_gcc_compiler.h"
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
#endif


namespace cpro {

MemoryGCCCompiler::MemoryGCCCompiler(void){

}

MemoryGCCCompiler::MemoryGCCCompiler(const std::string& mem_src_code)
    : mem_src_code_(mem_src_code) {
    this->gcc_option_.set_compiler("gcc");
    this->gcc_option_.set_output("temp_memory_compiler");
    this->gcc_option_.set_language("c");
    this->gcc_option_.set_g(true);
    this->gcc_option_.set_Wall(false);
    this->gcc_option_.set_optimization_level('0');

}

MemoryGCCCompiler::~MemoryGCCCompiler(void) = default;


void MemoryGCCCompiler::set_syntax_only(bool syntax_only){
    this->gcc_option_.set_syntax_only(syntax_only);
}

std::string MemoryGCCCompiler::get_mem_src_code(void) const{
    return this->mem_src_code_;
}

void MemoryGCCCompiler::set_mem_src_code(const std::string& mem_src_code){
    this->mem_src_code_ = mem_src_code;
}

int MemoryGCCCompiler::compile(void){
#if defined(_WIN32) || defined(_WIN64)
    // 创建管道
    SECURITY_ATTRIBUTES s_attr;
    s_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    s_attr.bInheritHandle = TRUE;
    s_attr.lpSecurityDescriptor = NULL;

    HANDLE h_sub_stdin_rd, h_sub_stdin_wr;
    if (!CreatePipe(&h_sub_stdin_rd, &h_sub_stdin_wr, &s_attr, 0)) {
        std::cerr << "CreatePipe failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 确保父进程的写句柄不被子进程继承
    if (!SetHandleInformation(h_sub_stdin_wr, HANDLE_FLAG_INHERIT, 0)) {
        std::cerr << "SetHandleInformation failed: " << GetLastError() << std::endl;
        CloseHandle(h_sub_stdin_rd);
        CloseHandle(h_sub_stdin_wr);
        return 1;
    }

    // 创建子进程
    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdInput = h_sub_stdin_rd; // 子进程从管道读取
    si.dwFlags |= STARTF_USESTDHANDLES;

    // std::string command_line = "gcc -x c -";
    std::string command_line = this->gcc_option_.generate_command();

    // 创建子进程
    BOOL bSuccess = CreateProcessA(
        NULL,
        &command_line[0],   // 命令行
        NULL,          // 进程安全属性
        NULL,          // 线程安全属性
        TRUE,          // 继承句柄
        0,             // 创建标志
        NULL,          // 环境块
        NULL,          // 当前目录
        &si,  // STARTUPINFO
        &pi    // PROCESS_INFORMATION
    );

    if (!bSuccess) {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        CloseHandle(h_sub_stdin_rd);
        CloseHandle(h_sub_stdin_wr);
        return 1;
    }

    // 关闭父进程不再使用的句柄
    CloseHandle(h_sub_stdin_rd);
    CloseHandle(pi.hThread);

    // 向子进程的标准输入写入数据
    DWORD bytesWritten;
    const char* data = this->mem_src_code_.c_str();
    DWORD dataSize = static_cast<DWORD>(this->mem_src_code_.size());

    if (!WriteFile(h_sub_stdin_wr, data, dataSize, &bytesWritten, NULL)) {
        std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
    } else if (bytesWritten != dataSize) {
        std::cerr << "Not all data written to pipe" << std::endl;
    }

    // 关闭管道写端，表示输入结束
    CloseHandle(h_sub_stdin_wr);

    // 等待子进程退出
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 获取子进程退出码
    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);

    return exit_code;
#else
    return 0;
#endif
}

} // namespace cpro