#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

#include "proj_info.h"
#include "help.h"
// #include "repl.h"
#include "memory_prog.h"
#include "subprocess.h"
#include "mod_proj.h"
#include "handle_cmd_run.h"
#include "handle_cmd_winres.h"
#include "table.h"
#include "scaffold.h"

void init(void);
int handle_cmd_env(const int argc, char* argv[]);
int handle_cmd_help(const int argc, char* argv[]);
int handle_cmd_proj(const int argc, char* argv[]);
int handle_cmd_repl(const int argc, char* argv[]);
int handle_cmd_version(const int argc, char* argv[]);

int main(const int argc, char* argv[]) {
	// 初始化
    init();

	// 命令路由表
	std::unordered_map<std::string, int(*)(int, char*[])> command_map = {
		{"env", handle_cmd_env},
		{"help", handle_cmd_help},
		{"proj", handle_cmd_proj},
		{"repl", handle_cmd_repl},
		{"run", cpro::handle_cmd_run},
		{"scaffold", cpro::handle_cmd_scaffold},
		{"table", cpro::handle_cmd_table},
		{"version", handle_cmd_version},
		{"winres", cpro::handle_cmd_winres}
	};

	// 检查参数数量
	if(argc < 2){
		binhub_cerr("Usage: %s <command> [options]", argv[0]); putchar('\n');
		std::cout << "Type `" << argv[0] << " help` for help." << std::endl;
		return EXIT_FAILURE;
	}

	// 获取子命令名称
	std::string command = argv[1];

	// 调整参数指针和计数
	char** new_argv = argv + 1;  // 跳过程序名
	int new_argc = argc - 1;

	// 执行子命令
	if (command_map.find(command) != command_map.end()) {
		return command_map[command](new_argc, new_argv);
	} else {
		binhub_cerr("Unknown command: %s", command.c_str()); putchar('\n');
		return 1;
	}
}

void init(void){
#ifdef _WIN32 // Linux 默认
	binhub_setenc(ENCODING_UTF8);
#endif
}

int handle_cmd_env(const int argc, char* argv[]){
	BINHUB_UNUSED(argc);
	BINHUB_UNUSED(argv);
	cpro::print_env();
	return 0;
}

int handle_cmd_help(const int argc, char* argv[]){
	if(argc == 1) {
		cpro::print_manual(nullptr);
	} else {
		cpro::print_manual(argv[1]);
	}
	return 0;
}

int handle_cmd_proj(const int argc, char* argv[]){
	BINHUB_UNUSED(argc);
	BINHUB_UNUSED(argv);

	cpro::ModProj mod_proj("new_proj", "c++");
	mod_proj.input_proj_info();
	mod_proj.create_cmake_project();
	return 0;
}

int handle_cmd_repl(const int argc, char* argv[]){
	cpro::MemoryProg mem_prog;
	return mem_prog.repl(argc, argv);
}

int handle_cmd_version(const int argc, char* argv[]){
	BINHUB_UNUSED(argc);
	BINHUB_UNUSED(argv);
	cpro::print_version();
	return 0;
}
