#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include <string>
#ifdef _WIN32
	#include <windows.h>
#else

#endif

namespace cpro{

class Subprocess {
private:
	STARTUPINFOA si;		// 启动信息
	PROCESS_INFORMATION pi; // 包含新进程的信息

	std::string subprocess_name_;
	std::string subprocess_args_;
	std::string cmd_line_;
	bool verbose_;

	bool start(void); // 启动子进程
	void wait_for_exit(void); // 等待子进程结束
	DWORD get_exit_code(void); // 获取子进程的退出代码

public:
	/**
     * @brief constructor
     * @param subprocess_name 子进程名称
     * @param subprocess_args 子进程参数
     * @param verbose
     */
	Subprocess(const std::string& subprocess_name, const std::string& subprocess_args, bool verbose = false);
	~Subprocess(void);
	bool exec(void);
};

};

#endif