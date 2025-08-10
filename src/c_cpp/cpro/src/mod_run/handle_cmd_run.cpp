#include "handle_cmd_run.h"
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_set>
#include <filesystem>
#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif
#include "subprocess.h"
#include "gen_template.h"
#include "gcc_option.h"

namespace cpro {

// run <options> <sources>
int handle_cmd_run(const int argc, char* argv[]){
	// 0. 初始化 gcc 选项
	cpro::GCCOption gcc_option;
	gcc_option.set_compiler("gcc");
	gcc_option.set_language("none"); // 语言默认为 none, 即不指定语言(为了添加 resource 文件)
	gcc_option.set_g(true);
	gcc_option.set_Wall(false);
	gcc_option.set_optimization_level('0');

	// 1. 解析参数
	if(argc < 2){
		binhub_cerr("Usage: cpro %s <options> <sources>", argv[0]); putchar('\n');
		std::cout << "Type `cpro help run` for help." << std::endl;
		return 1;
	}
	RunOptions options;
	(void)options.analyze_args(argc, argv); // 解析参数

	// 2. 设置输出文件名和源文件名（windows resources 可以保存在 sources 中）
	std::string exe_filename = options.get_output();
	if(exe_filename.empty()){
#ifdef _WIN32
		exe_filename = "a.exe";
#else
		exe_filename = "a.out";
#endif
	}
	std::set<std::string> sources = options.get_sources();

	// 3. 编译源文件
	for(const auto& source : sources){
		if(!std::filesystem::exists(source) || !std::filesystem::is_regular_file(source)){
			binhub_cerr("Error! Source file not found: %s", source.c_str());
			return 1;
		}
		gcc_option.append_src_files(source);
	}
	if(sources.empty()){
		binhub_cerr("Error! No source file specified.");
		return 1;
	}

	try {
		gcc_option.set_output(exe_filename);
		std::string compile_command = gcc_option.generate_command();
        // std::cout << "gcc command: " << compile_command << std::endl;
		system(compile_command.c_str());
	} catch (const std::exception& e) {
		binhub_cerr("Error! An error occurred while compiling the program.");
	}

	// 4. 执行程序
	if(std::filesystem::exists(exe_filename) && std::filesystem::is_regular_file(exe_filename)){ // 文件不存在
		cpro::Subprocess subprocess(exe_filename, options.get_sub_args(), true);
		subprocess.exec();
	} else {
		binhub_cerr("Error! Failed to find the executable file.");
		return 1;
	}
	return 0;
}

/************************** class RunOptions impl ********************************/

void RunOptions::input_sources_from_dir(void){

}

// public
RunOptions::RunOptions(void){
	this->sub_args_ = std::make_pair(false, "");
	this->output_ = std::make_pair(false, "");
}

RunOptions::~RunOptions(void) = default;

std::string RunOptions::get_sub_args(void){
	if(this->sub_args_.first){
		return this->sub_args_.second;
	} else {
		return "";
	}
}

std::string RunOptions::get_output(void){
	if(this->output_.first){
		return this->output_.second;
	} else {
		return "";
	}
}

std::set<std::string> RunOptions::get_sources(void){
	return this->sources_;
}

int RunOptions::analyze_args(int argc, char* argv[]){
	for(int i = 1; i < argc; i++){ // 忽略 argv[0]
		if(argv[i][0] == '-'){ // - 参数
			if(!strcmp("-a", argv[i])){ // sub_args
				this->sub_args_.first = true;
			} else if(!strcmp("-o", argv[i])){
				this->output_.first = true;
				if(argv[i+1][0] != '-' && argv[i+1][0] != '\0'){ // -o 后面不以 '-' 为首的第一个参数为 output_
					this->output_.second = argv[i+1];	// output
					++i;
				}
			} else {
				binhub_cerr("Unknow options: %s", argv[i]); putchar('\n');
			}
		} else {
			this->sources_.insert(argv[i]);
		}
	}

	// 设置了 -a 参数
    if(this->sub_args_.first){
    	printf("Subprocess options> ");
    	if (!std::getline(std::cin, this->sub_args_.second)) {  // 检查 子进程参数 读取是否成功
    		binhub_cerr("Error: Failed to read input."); putchar('\n');
    	}
	}

	// 设置了 -o 参数但未在参数列表中赋值
	if(this->output_.first && this->output_.second.empty()){
		printf("Output filename> ");
		if (!std::getline(std::cin, this->output_.second)) {  // 检查读取是否成功
			binhub_cerr("Error: Failed to read input."); putchar('\n');
#ifdef _WIN32
			this->output_.second = "a.exe";
#else
			this->output_.second = "a.out";
#endif
		}
	}
	return 0;
}



} // namespace cpro