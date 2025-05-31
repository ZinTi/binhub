#ifndef SUBPROCESS_INC
#define SUBPROCESS_INC
#include <windows.h>

class ChildProcess {
	private:
		STARTUPINFOA si;		// 启动信息
		PROCESS_INFORMATION pi; // 包含新进程的信息
		char* subproName;
		char* subproArgs;
		char* cmdLine;
		bool verbose;

		bool start(); // 启动子进程，参数为命令行
		void waitForExit(); // 等待子进程结束
		DWORD getExitCode(); // 获取子进程的退出代码
	public:
		ChildProcess(const char* newSubproName = nullptr, const char* newSubproArgs = nullptr, bool newVerbose = false);
		~ChildProcess();
		bool execSubprog(void);
};

#endif